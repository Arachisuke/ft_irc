/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/09/01 14:54:59 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Server.hpp"

Server::Server()
{
    this->_serverName = "HuecoMundo";
    this->fd = -1;
    this->load_cmd();
    this->port = 0;
}

void Server::Finish()
{
    for (size_t i = 0; i < this->clientList.size(); i++)
    {
        if (this->clientList[i] && this->clientList[i]->fd != -1)
            delete this->clientList[i]; // delete le client qui appel le destructeur qui lui close le fd.
    }
    close(this->fd);
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
    this->hote.sin_family = AF_INET;
    this->hote.sin_port = htons(this->port);
    this->hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&hote);
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw(std::runtime_error("ERR_FD"));
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        close(this->fd);
        throw(std::runtime_error("ERR_SETSOCKOPT"));
    }
    if (bind(this->fd, reinterpret_cast<struct sockaddr *>(&this->hote), sizeof(this->hote)) == -1)
    {
        close(this->fd);
        throw(std::runtime_error("ERR_BIND"));
    }
    if (listen(this->fd, 10) == -1)
    {
        close(this->fd);
        throw(std::runtime_error("ERR_LISTEN"));
    }
    fcntl(fd, F_SETFL, O_NONBLOCK);
    this->events[0].events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->events[0].data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->events[0]);
    return (0);
}

void Server::closeClient(std::string ERROR_MSG)
{
    if (!ERROR_MSG.empty())
        this->PushMsg(ERROR_MSG); // MSG ERROR push est au norme de IRC en type de msg.
    if (this->clientList[this->nbrclient])
    {
        if (this->clientList[this->nbrclient]->fd > 0)
        {
            {
                close(this->clientList[this->nbrclient]->fd);
                epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->clientList[this->nbrclient]->fd, NULL); // CTLDEL
            }
        }
        delete this->clientList[this->nbrclient];                           // DELETE + (CLOSE + CTLDEL proteger par le if fd > 0)
        this->clientList.erase(this->clientList.begin() + this->nbrclient); // ERASE
    }
    return;
}
// Server Server();

void Server::load_cmd() // sans pass
{
    
    commandList["NICK"] = &Server::nick; // valide
    commandList["USER"] = &Server::user; // valide
    commandList["QUIT"] = &Server::quit;
    commandList["JOIN"] = &Server::join; // moi
    commandList["PART"] = &Server::part;
    commandList["PRIVMSG"] = &Server::privMsg; // moi
    commandList["NOTICE"] = &Server::notice; // moi 
    commandList["MODE"] = &Server::mode; //duo
    commandList["TOPIC"] = &Server::topic; //andy
    commandList["INVITE"] = &Server::invite; //andy
    commandList["KICK"] = &Server::kick; //andy
    commandList["PING"] = &Server::ping; // valide 
    commandList["PASS"] = &Server::pass; // valide 
}

void Server::create_server(int port, char *password)
{
    this->port = port;
    this->epfd = epoll_create1(0);
    this->password = password;
    this->Init();
}

int Server::find_client(std::string &nameClient)
{
    if (this->clientList.empty())
        return (-1);
    for (size_t i = 0; i < this->clientList.size(); i++)
    {
        if (this->clientList[i]->nickname == nameClient)
            return (i);
    }
    return (-1);
}

int Server::find_client(int fd)
{
    if (this->clientList.empty())
        return (-1);
    for (size_t i = 0; i < this->clientList.size(); i++)
    {
        if (this->clientList[i]->fd == fd)
            return (i);
    }
    return (-1);
}
int Server::wait_client()
{
    int nfds;

    nfds = epoll_wait(this->epfd, this->events, MAX_EVENTS, -1);
    if (nfds == -1)
        throw(std::runtime_error("ERR_EPOLLWAIT"));
    for (int i = 0; i < nfds; ++i)
    {
        if (this->events[i].data.fd == this->fd) // new client
        {
            Client *Clients = new Client();
            if (Clients->Init(this->epfd, this->fd))
            {
                delete Clients;
                std::cerr << "Failed to accept new client" << std::endl;
                continue;
            }
            this->clientList.push_back(Clients);
            std::cout << "New client connected" << std::endl;
        }
        else if (this->events[i].data.fd != this->fd)
        {
            nbrclient = find_client(this->events[i].data.fd); // correction erreur assignation nbrclient
            if (this->events[i].events == EPOLLIN)
                this->ReadMsg(this->clientList[this->nbrclient]->buffer);
            else if (this->events[i].events == EPOLLOUT)
                this->PushMsg("MON MSG");
            else if (this->events[i].events == EPOLLHUP)
                this->closeClient("HUP");
            else if (this->events[i].events == EPOLLRDHUP)
                this->closeClient("RDHUP");
            else if (this->events[i].events == EPOLLERR)
                this->closeClient("ERR");
        }
    }
    return 0;
}


void Server::ReadMsg(std::string& bufferClient)
{

    char lecture[512];

    this->bytes = recv(this->clientList[this->nbrclient]->fd, &lecture, sizeof(lecture), MSG_DONTWAIT);
    if (this->bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        return (this->closeClient("ERR_READ"));
    }
    else if (this->bytes == 0)
        return (this->closeClient("ERR_READ"));
    else if (this->bytes > 0)
    {
        bufferClient.append(lecture, this->bytes); // marche aps.
        size_t pos = bufferClient.find("\r\n");
        while (pos != std::string::npos)
        {
            this->entry = bufferClient.substr(0, pos);
            bufferClient.erase(0, pos + 2);
            this->find_cmd();
            pos = bufferClient.find("\r\n");
        }
    }
}

void Server::find_cmd()
{

    std::string word;
    std::istringstream iss(this->entry); // gerer le parse ":" // alias relire la doc du parse
    while (iss >> word)
        this->cmd.push_back(word);
    
    for (std::map<std::string, CommandFunc>::iterator it = commandList.begin(); it != commandList.end(); ++it)
    {

        if (this->cmd[0] == it->first)
        {
            (this->*(it->second))();
            this->cmd.clear();
            return;
        }
    }
    if (this->cmd[0] == "CAP")
        return ;
    this->cmd.clear();
    std::cout << "Command not found" << std::endl;
}

void Server::PushMsg(std::string msg) // a gerer apres
{
    msg.push_back('\r');
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(this->clientList[this->nbrclient]->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
    this->events[nbrclient].events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->events[nbrclient].data.fd = this->clientList[nbrclient]->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->events[nbrclient]);
}


void Server::reply(int codeError, const std::string command, const std::string message, Client &client) const
{
    std::ostringstream ost;
    ost << ":" << this->_serverName << " " << codeError << " " << client.nickname << " " << command << " :" << message << "\r\n";
    send(client.fd, ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
}