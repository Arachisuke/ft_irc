/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/08/21 16:01:22 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Server.hpp"

Server::Server()
{
    this->fd = -1;
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

int Server::Init(int epfd, int port)
{

    int yes = 1;
    this->hote.sin_family = AF_INET;
    this->hote.sin_port = htons(port);
    this->hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&hote);
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw(std::runtime_error("ERR_FD"));
    // return(std::cout << "ERR_FD" << std::endl, -1);
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        close(this->fd);
        throw(std::runtime_error("ERR_SETSOCKOPT"));
        // return(close(this->fd), 50);
    }
    if (bind(this->fd, reinterpret_cast<struct sockaddr *>(&this->hote), sizeof(this->hote)) == -1)
    {
        close(this->fd);
        throw(std::runtime_error("ERR_BIND"));
        // return(close(this->fd),  std::cout << "ERR_BIND" << std::endl, -1);
    }
    if (listen(this->fd, 10) == -1)
    {
        close(this->fd);
        throw(std::runtime_error("ERR_LISTEN"));
        // return(close(this->fd), std::cout << "ERR_LISTEN" << std::endl, -1);
    }
    fcntl(fd, F_SETFL, O_NONBLOCK);
    this->epfd = epfd;
    this->events[0].events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->events[0].data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->events[0]);
    return (0);
}

void Server::closeClient(int nbrclient, std::string ERROR_MSG)
{
    if (!ERROR_MSG.empty())
        this->PushMsg(nbrclient, ERROR_MSG); // MSG ERROR push est au norme de IRC en type de msg.
    if (this->clientList[nbrclient])
    {
        delete this->clientList[nbrclient];                           // DELETE + (CLOSE + CTLDEL proteger par le if fd > 0)
        this->clientList.erase(this->clientList.begin() + nbrclient); // ERASE
    }
    std::cout << "Client disconnected\r\n"
              << std::endl;
    return;
}
Server Server();

void Server::load_cmd() // sans pass
{
    // commandList["CAP"] = &Server::cap(this->cmd); // a ignorer.
    commandList["NICK"] = &Server::nick;
    commandList["USER"] = &Server::user;
    commandList["QUIT"] = &Server::quit; // en plus du prototype ici il faut verifie que le Server est bien connecte et dans le bon contexte.
    commandList["JOIN"] = &Server::join;
    commandList["PART"] = &Server::part;
    commandList["PRIVMSG"] = &Server::privMsg;
    commandList["NOTICE"] = &Server::notice;
    commandList["MODE"] = &Server::mode;
    commandList["TOPIC"] = &Server::topic;
    commandList["INVITE"] = &Server::invite;
    commandList["KICK"] = &Server::kick;
    commandList["PING"] = &Server::ping;
}

int Server::create_server(int port, char *password) // mettre le truc de reference.
{
    int epfd;
    epfd = epoll_create1(0);
    this->password = password;

    if (this->Init(epfd, port)) // gerer les erreurs.
        return (-1);
    return 0;
}

int Server::find_client(std::vector<Client *> client_list, int fd)
{
    if (client_list.empty())
        return (-1);
    for (size_t i = 0; i < client_list.size(); i++)
    {
        if (client_list[i]->fd == fd)
            return (i);
    }
    return (-1);
}
int Server::wait_client()
{
    int nfds;
    int nbrclient;

    nfds = epoll_wait(this->epfd, this->events, MAX_EVENTS, -1); // -1 ?
    if (nfds == -1)                                              // que faire ?
        this->Finish();
    throw(std::runtime_error("ERR_EPOLLWAIT"));

    for (int i = 0; i < nfds; ++i)
    {
        if (nfds && this->events[i].data.fd == this->fd) // new client
        {
            Client *Clients = new Client();
            if (Clients->Init(this->epfd, this->fd))
            {
                delete Clients;
                std::cerr << "Failed to accept new client" << std::endl;
                continue;
            }
            this->clientList.push_back(Clients);
            std::cout << "New client connected\r\n"
                      << std::endl;
        }
        else if (nfds && this->events[i].data.fd != this->fd)
        {
            nbrclient = find_client(this->clientList, this->events[i].data.fd);
            if (this->events[i].events == EPOLLIN)
                this->ReadMsg(nbrclient, clientList[nbrclient]->buffer); // changer read en lui donnant le bon client via clientList et findclient
            else if (this->events[i].events == EPOLLOUT)
                this->PushMsg(nbrclient, "MON MSG");
            else if (this->events[i].events == EPOLLHUP)
                this->closeClient(nbrclient, "HUP"); // change le big 3, par une fonction dans le serveur qui nettoie tout.
            else if (this->events[i].events == EPOLLRDHUP)
                this->closeClient(nbrclient, "RDHUP");
            else if (this->events[i].events == EPOLLERR)
                this->closeClient(nbrclient, "ERR"); // change le big 3, par une fonction dans le serveur qui nettoie tout.
        }
        return 0;
    }
}
void Server::find_cmd() // le client c qui
{

    std::string word;
    int i = 0;
    std::istringstream iss(this->entry);
    while (iss >> word) // a verifier
    {
        this->cmd[i] = word;
        i++;
    }
    for (std::map<std::string, CommandFunc>::iterator it = commandList.begin(); it != commandList.end(); ++it)
    {
        if (cmd[0] == it->first)
        {
            (this->*(it->second))();
            return;
        }
    }
    std::cout << "Command not found" << std::endl;
}

void Server::ReadMsg(int nbrclient, std::string bufferClient)
{

    char lecture[512];

    this->bytes = recv(this->fd, &lecture, sizeof(lecture), MSG_DONTWAIT);
    if (this->bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // pas de message a lire
            return (std::cout << "EAGAIIIIIIN" << std::endl, (void)0);
        return (this->closeClient(nbrclient, "ERR_READ"));
    }
    else if (this->bytes == 0)
        return (this->closeClient(nbrclient, "ERR_READ"));
    else if (this->bytes > 0)
    {
        bufferClient.append(lecture, this->bytes);
        size_t pos = bufferClient.find("\r\n");
        while (pos != std::string::npos)
        {
            this->entry = bufferClient.substr(0, pos);
            bufferClient.erase(0, pos + 2);
            this->executeOrNot();
            pos = bufferClient.find("\r\n");
        }
    }
}

// chaque debut de fonction
// verifier le nombre de params, et si le param correspond a la police par exemple ou a ce qui est attendu.
// bien connecte.
// bien dans le bon contexte.

void Server::PushMsg(int nbrclient, std::string msg) // a gerer apres
{
    msg.push_back('\r');
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(this->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
    this->events[nbrclient].events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->events[nbrclient].data.fd = this->clientList[nbrclient]->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->events[nbrclient]);
}