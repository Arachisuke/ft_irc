/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/09/04 17:21:40 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Server.hpp"

Server::Server()
{
    this->_serverName = "HuecoMundo";
    this->_fd = -1;
    this->load_cmd();
    this->_port = 0;
}

void Server::Finish()
{
    for (size_t i = 0; i < this->_clientList.size(); i++)
    {
        if (this->_clientList[i] && this->_clientList[i]->getFd() != -1)
        {
            this->_nbrclient = i;
            this->closeClient("");
        }
    }
    for (size_t i = 0; i < this->_channeList.size(); i++)
        {
            if (this->_channeList[i])
                delete this->_channeList[i];   
        }
    // clear si tu veux les containers. il se clear en vrai a la fin..auto.
    if (this->_fd > 0)
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


// Server Server();

void Server::load_cmd() // sans pass
{

    _commandList["NICK"] = &Server::nick; // valide
    _commandList["USER"] = &Server::user; // valide
    _commandList["QUIT"] = &Server::quit; // pas fait
    _commandList["JOIN"] = &Server::join; // valide manque le mode
    _commandList["PART"] = &Server::part; // valide 
    _commandList["PRIVMSG"] = &Server::privMsg; // valide 
    _commandList["NOTICE"] = &Server::notice;   // valider
    _commandList["MODE"] = &Server::mode;       // a faire
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

int Server::find_client(std::string &nameClient) // une reference ?
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
        if (this->_events[i].data.fd == this->_fd) 
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
            _nbrclient = find_client(this->_events[i].data.fd); 
            if (_nbrclient == -1) 
                continue;   
            if (this->_events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
            {
                if (this->_events[i].events & EPOLLHUP)
                    this->closeClient("HUP");
                else if (this->_events[i].events & EPOLLRDHUP)
                    this->closeClient("RDHUP");
                else if (this->_events[i].events & EPOLLERR)
                    this->closeClient("ERR");
            }
            else if (this->_events[i].events & EPOLLIN)
            {
                try
                {
                    this->ReadMsg(this->_clientList[this->_nbrclient]->setBuffer());
                }
                catch (std::exception &e)
                {
                    this->PushMsg(e.what());
                    this->closeClient("");
                }
            }
            else if (this->_events[i].events & EPOLLOUT) // gerer le epollout
                this->PushMsg("MON MSG");
        }
    }
    return 0;
}

void Server::closeClient(std::string ERROR_MSG)
{
    if (!ERROR_MSG.empty())
        this->PushMsg(ERROR_MSG); // MSG ERROR push est au norme de IRC en type de msg.
    if (this->_clientList[this->_nbrclient]->getFd() > 0)
    {            
            close(this->_clientList[this->_nbrclient]->getFd());
            epoll_ctl(this->_epfd, EPOLL_CTL_DEL, this->_clientList[this->_nbrclient]->getFd(), NULL);
    }

    delete this->_clientList[this->_nbrclient];                            
    this->_clientList.erase(this->_clientList.begin() + this->_nbrclient); // ERASE

    return;
}

void Server::ReadMsg(std::string& bufferClient)
{

    char lecture[512];

    this->_bytes = recv(this->_clientList[this->_nbrclient]->getFd(), &lecture, sizeof(lecture), MSG_DONTWAIT);
    if (this->_bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        return (this->closeClient("ERR_READ"));
    }
    else if (this->_bytes == 0)
        return (this->closeClient("ERR_READ"));
    else if (this->_bytes > 0)
    {
        bufferClient.append(lecture, this->_bytes); // marche aps.
        size_t pos = bufferClient.find("\r\n");
        while (pos != std::string::npos)
        {
            this->_entry = bufferClient.substr(0, pos);
            bufferClient.erase(0, pos + 2);
            this->find_cmd();
            pos = bufferClient.find("\r\n");
        }
    }
}

void Server::find_cmd()
{
    std::string word;
    std::string concat;
    int i = 0;
    std::istringstream iss(this->_entry); 
    this->_cmd.clear();
    while (iss >> word)
    {
        if (word[0] == ':' || i == 15)
        {
            if (word[0] == ':')
                word = word.substr(1);
            concat += word;
            while (iss >> word)
                concat += ' ' + word;
            this->_cmd.push_back(concat);
        }
        else
            this->_cmd.push_back(word);
        i++;
    }
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
    send(this->_clientList[this->_nbrclient]->getFd(),"Command not found\r\n", 20, MSG_DONTWAIT);
}

void Server::PushMsg(std::string msg)  // sers plus a rien. juste a rajoute les normes.
{
    msg.push_back('\r');
    msg.push_back('\n'); 
    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
}


void Server::reply(int codeError, const std::string command, const std::string message, Client &client) const
{
    std::ostringstream ost;
    ost << ":" << this->_serverName << " " << codeError << " " << client.getNickname() << " " << command << " :" << message << "\r\n";
    send(client.getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
}