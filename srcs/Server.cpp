/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/09/09 14:43:04 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Server.hpp"

Server::Server() : _serverName("HuecoMundo"), _fd(-1), _RPL_WELCOME(0), _bytes(-1), _nbrclient(-1), _port(0)
{
    this->load_cmd();
}

void Server::Finish()
{
    for (size_t i = 0; i < this->_clientList.size(); i++)
    {
        if (this->_clientList[i] && this->_clientList[i]->getFd() != -1)
            delete this->_clientList[i]; // delete le client qui appel le destructeur qui lui close le fd.
    }
    close(this->_fd);
}
Server::~Server()
{

    this->Finish();
    std::cout << "Server shutdown" << std::endl;
}

void clearsinzero(struct sockaddr_in *addr)

{
    for (int i = 0; i < 8; ++i)
        addr->sin_zero[i] = 0;
}

int Server::Init()
{

    int yes = 1;
    this->_hote.sin_family = AF_INET;
    this->_hote.sin_port = htons(this->_port);
    this->_hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&_hote);
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
        throw(std::runtime_error("ERRFD)"));
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        close(this->_fd);
        throw(std::runtime_error("ERR_SETSOCKOPT"));
    }
    if (bind(this->_fd, reinterpret_cast<struct sockaddr *>(&this->_hote), sizeof(this->_hote)) == -1)
    {
        close(this->_fd);
        throw(std::runtime_error("ERR_BIND"));
    }
    if (listen(this->_fd, 10) == -1)
    {
        close(this->_fd);
        throw(std::runtime_error("ERR_LISTEN"));
    }
    fcntl(_fd, F_SETFL, O_NONBLOCK);
    this->_events[0].events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->_events[0].data.fd = this->_fd;
    epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_fd, &this->_events[0]);
    return (0);
}

void Server::closeClient(std::string ERROR_MSG)
{
    if (!ERROR_MSG.empty())
        this->PushMsg(ERROR_MSG); // MSG ERROR push est au norme de IRC en type de msg.
    if (this->_clientList[this->_nbrclient])
    {
        if (this->_clientList[this->_nbrclient]->getFd() > 0)
        {
            {
                std::cout << "client : " << this->_clientList[this->_nbrclient]->getFd() << " disconnected" << std::endl;
                close(this->_clientList[this->_nbrclient]->getFd());
                epoll_ctl(this->_epfd, EPOLL_CTL_DEL, this->_clientList[this->_nbrclient]->getFd(), NULL); // CTLDEL
            }
        }
        delete this->_clientList[this->_nbrclient];                            // DELETE + (CLOSE + CTLDEL proteger par le if fd > 0)
        this->_clientList.erase(this->_clientList.begin() + this->_nbrclient); // ERASE
    }
    return;
}
// Server Server();

void Server::load_cmd() // sans pass
{

    _commandList["NICK"] = &Server::nick; // valide
    _commandList["USER"] = &Server::user; // valide
    _commandList["QUIT"] = &Server::quit;
    _commandList["JOIN"] = &Server::join; // moi
    _commandList["PART"] = &Server::part;
    _commandList["PRIVMSG"] = &Server::privMsg; // moi
    _commandList["NOTICE"] = &Server::notice;   // moi
    _commandList["MODE"] = &Server::mode;       // duo
    _commandList["TOPIC"] = &Server::topic;     // andy
    _commandList["INVITE"] = &Server::invite;   // andy
    _commandList["KICK"] = &Server::kick;       // andy
    _commandList["PING"] = &Server::ping;       // valide
    _commandList["PASS"] = &Server::pass;       // valide
}

void Server::create_server(int port, char *password)
{
    this->_port = port;
    this->_epfd = epoll_create1(0);
    this->_password = password;
    this->Init();
}

int Server::find_client(std::string &nameClient)
{
    if (this->_clientList.empty())
        return (-1);
    for (size_t i = 0; i < this->_clientList.size(); i++)
    {
        if (this->_clientList[i]->getNickname() == nameClient)
            return (i);
    }
    return (-1);
}

int Server::find_client(int fd)
{
    if (this->_clientList.empty())
        return (-1);
    for (size_t i = 0; i < this->_clientList.size(); i++)
    {
        if (this->_clientList[i]->getFd() == fd)
            return (i);
    }
    return (-1);
}
int Server::wait_client()
{
    int nfds;

    nfds = epoll_wait(this->_epfd, this->_events, MAX_EVENTS, -1);
    if (nfds == -1)
        throw(std::runtime_error("ERR_EPOLLWAIT"));
    for (int i = 0; i < nfds; ++i)
    {
        if (this->_events[i].data.fd == this->_fd) // new client
        {
            Client *client = new Client();
            if (client->Init(this->_epfd, this->_fd))
            {
                delete client;
                std::cerr << "Failed to accept new client" << std::endl;
                continue;
            }
            this->_clientList.push_back(client);
            std::cout << "New client connected" << std::endl;
        }
        else if (this->_events[i].data.fd != this->_fd)
        {
            _nbrclient = find_client(this->_events[i].data.fd); // correction erreur assignation nbrclient
            if (_nbrclient == -1)                               // Client non trouvé, ignore
                continue;

            // Vérifier les événements de déconnexion en premier
            if (this->_events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
            {
                if (this->_events[i].events & EPOLLHUP)
                {
                    std::cout << "HUP" << std::endl;
                    this->closeClient("HUP");
                }
                else if (this->_events[i].events & EPOLLRDHUP)
                {
                    std::cout << "RDHUP" << std::endl;
                    this->closeClient("RDHUP");
                }
                else if (this->_events[i].events & EPOLLERR)
                {
                    std::cout << "ERR" << std::endl;
                    this->closeClient("ERR");
                }
            }
            else if (this->_events[i].events & EPOLLIN)
                this->ReadMsg(this->_clientList[this->_nbrclient]->setBuffer());
            else if (this->_events[i].events & EPOLLOUT)
                this->PushMsg("MON MSG");
        }
    }
    return 0;
}

void Server::ReadMsg(std::string &bufferClient)
{

    char lecture[512];
    this->_bytes = recv(this->_clientList[this->_nbrclient]->getFd(), &lecture, sizeof(lecture), MSG_DONTWAIT);
    if (this->_bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
                    
        return (std::cout << "ERR_READ 11111" << std::endl, this->closeClient("ERR_READ"));
    }
    else if (this->_bytes == 0)
        return (std::cout << "ERR_READ 22221" << std::endl, this->closeClient("ERR_READ"));
    else if (this->_bytes > 0)
    {
        bufferClient.append(lecture, this->_bytes); // marche aps.
        size_t pos = bufferClient.find("\r\n");
        while (pos != std::string::npos)
        {
            this->_entry = bufferClient.substr(0, pos);
            bufferClient.erase(0, pos + 2);
            this->find_cmd();
            this->_entry.clear();
            pos = bufferClient.find("\r\n");
        }
    }
}

void removePrefix(std::string &wordPrefixLess)
{
    if (wordPrefixLess[0] == ':')
    {
        size_t spacePos = wordPrefixLess.find(' ');
        if (spacePos != std::string::npos)
            wordPrefixLess = wordPrefixLess.substr(spacePos + 1);
    }
    return;
}

void Server::parseCmd(std::string &wordPrefixLess)
{
    bool isTrailing = 0;
    std::istringstream iss(wordPrefixLess); // gerer le parse ":" // alias relire la doc du parse
    std::string finalWord;
    while (iss >> finalWord)
    {
        if (finalWord[0] == ':')
        {
            isTrailing = 1;
            break;
        }
        else
        {
            if (this->_cmd.size() < 14)
                this->_cmd.push_back(finalWord);
            else
            {
                isTrailing = 1;
                break;
            }
        }
    }
    if (isTrailing)
    {
        std::string trailing = finalWord;
        if (trailing[0] == ':')
            trailing = trailing.substr(1);
        std::string rest;
        std::getline(iss, rest);
        trailing += rest;
        this->_cmd.push_back(trailing);
    }
}

void Server::printParsedCmd()
{
    int i = 0;
    std::cout << "client: " << _clientList[_nbrclient]->getNickname() << " " << _clientList[_nbrclient]->getFd() << " send" << std::endl;
    std::cout << "============ Original Entry ============" << std::endl
              << _entry << std::endl
              << std::endl;
    std::cout << "============ Parsed Command ============" << std::endl
              << std::endl;
    for (std::vector<std::string>::const_iterator it = _cmd.begin(); it < _cmd.end(); it++)
        std::cout << "cmd[" << i++ << "]: " << (*it) << std::endl;
    std::cout << std::endl;
}

void Server::find_cmd()
{
    std::string wordPrefixLess = _entry;
    this->_cmd.clear();
    removePrefix(wordPrefixLess);
    parseCmd(wordPrefixLess);
    printParsedCmd();
    if (this->_cmd.empty())
        return;
    std::map<std::string, CommandFunc>::iterator it = _commandList.find(this->_cmd[0]);
    if (it != _commandList.end())
    {
        (this->*(it->second))();
        return;
    }
    if (this->_cmd[0] == "CAP")
        return;
    send(this->_clientList[this->_nbrclient]->getFd(), "Command not found\r\n", 20, MSG_DONTWAIT);
}

void Server::PushMsg(std::string msg) // a gerer apres
{
    msg.push_back('\r');
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
    this->_events[_nbrclient].events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->_events[_nbrclient].data.fd = this->_clientList[_nbrclient]->getFd();
    epoll_ctl(this->_epfd, EPOLL_CTL_MOD, this->_fd, &this->_events[_nbrclient]);
}

void Server::reply(int codeError, const std::string command, const std::string message, Client &client) const
{
    std::ostringstream ost;
    ost << ":" << this->_serverName << " " << codeError << " " << client.getNickname() << " " << command << " :" << message << "\r\n";
    send(client.getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
}

const std::string Server::getPrefiksServer() const
{
    return (":" + this->_serverName);
}

void Server::Send_Welcome() // rajouter le message 2 3 4.
{
    std::ostringstream ost;
    ost << ":" << this->_serverName << " 001 " << this->_clientList[_nbrclient]->getNickname() << " :Welcome to the Hueco Mundo Network, " << this->_clientList[_nbrclient]->getNickname() << "!~" << this->_clientList[_nbrclient]->getUsername() << "@localhost\r\n";
    send(this->_clientList[_nbrclient]->getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
    ost.str("");
    ost.clear();
    this->_RPL_WELCOME = 1;
    ost << ":" << this->_serverName << " 002 " << this->_clientList[_nbrclient]->getNickname() << " :Your host is " << this->_serverName << ", running version 4.3.3\r\n";
    send(this->_clientList[_nbrclient]->getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
    ost.str("");
    ost.clear();
    ost << ":" << this->_serverName << " 003 " << this->_clientList[_nbrclient]->getNickname() << " :This server was created " << __DATE__ << "\r\n";
    send(this->_clientList[_nbrclient]->getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
    ost.str("");
    ost.clear();
    ost << ":" << this->_serverName << " 004 " << this->_clientList[_nbrclient]->getNickname() << " " << this->_serverName << " version-4.3.3 itkol :are supported by this server\r\n";
    send(this->_clientList[_nbrclient]->getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
}

bool Server::checkChannelNorm(const std::string &channelName) const
{
    if (channelName[0] != '#')
        return (0);
    if (channelName.length() > 50 || channelName.length() == 1)
        return (0);
    for (size_t i = 0; i < channelName.length(); i++)
    {
        if (channelName[i] == ' ' || channelName[i] == ',' || channelName[i] == '\r' || channelName[i] == '\n' || channelName[i] == '\0')
            return (0);
    }
    return (1);
}


