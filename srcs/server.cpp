/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/08/21 13:16:14 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/server.hpp"

Server::Server()
{
    this->fd = -1;
}

void Server::Finish()
{
    for (size_t i = 0; i < this->client_list.size(); i++)
    {
        if (this->client_list[i] && this->client_list[i]->fd != -1)
            delete this->client_list[i]; // delete le client qui appel le destructeur qui lui close le fd.
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
    this->events[0].events = EPOLLIN;
    this->events[0].data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->events[0]);
    return (0);
}

Server::closeClient(int nbrclient, std::string ERROR_MSG)
{
    if (!ERROR_MSG.empty())
        this->clientList[nbrclient]->PushMsg(ERROR_MSG); // MSG ERROR push est au norme de IRC en type de msg.
    if (this->clientList[nbrclient])
    {
        delete this->clientList[nbrclient];                           // DELETE + (CLOSE + CTLDEL proteger par le if fd > 0)
        this->clientList.erase(this->clientList.begin() + nbrclient); // ERASE
    }
    std::cout << "Client disconnected\r\n"
              << std::endl;
    return;
}