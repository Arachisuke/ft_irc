/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/08/11 15:12:47 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{}
Server::~Server() {
    // Close all client sockets a faire
    // Close the server socket
    close(this->fd);
    std::cout << "Server shutdown" << std::endl;
}

int Server::Init(int epfd)
{
    this->hote.sin_family = AF_INET;
    this->hote.sin_port = htons(6667);
    this->hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&hote);
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        return(std::cout << "ERR_FD" << std::endl, 50);
    fcntl(fd, F_SETFL, O_NONBLOCK); // Rendre non bloquant
    this->epfd = epfd;
    this->events.events = EPOLLIN;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->events);
    return (0); // ajout du return en cas de succes
}
void Server::clearsinzero(struct sockaddr_in *addr) // ajout de la fonction sinzero initialement dans le main
{
    for (int i = 0; i < 8; ++i)
        addr->sin_zero[i] = 0;
}