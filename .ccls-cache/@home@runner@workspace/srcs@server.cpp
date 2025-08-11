/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:42:01 by macos             #+#    #+#             */
/*   Updated: 2025/08/10 16:45:16 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/server.hpp"

Server::Server()
{}
Server::~Server() {
    // Close all client sockets a faire
    // Close the server socket
    close(this->fd);
    std::cout << "Server shutdown" << std::endl;
}

void clearsinzero(struct sockaddr_in *addr)
{
    for (int i = 0; i < 8; ++i)
        addr->sin_zero[i] = 0;
}

int Server::Init(int epfd)
{
    int yes = 1;
    this->hote.sin_family = AF_INET;
    this->hote.sin_port = htons(6667);
    this->hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&hote);
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        return(std::cout << "ERR_FD" << std::endl, 50);
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
        return(close(this->fd), 50);

    if (bind(this->fd, reinterpret_cast<struct sockaddr*>(&this->hote), sizeof(this->hote)) == -1)
        return(close(this->fd),  std::cout << "ERR_BIND" << std::endl, 50);

    if(listen(this->fd, 10) == -1)
        return(close(this->fd), std::cout << "ERR_LISTEN" << std::endl, 50);
    
    fcntl(fd, F_SETFL, O_NONBLOCK); // Rendre non bloquant
    this->epfd = epfd;
    this->events[0].events = EPOLLIN;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->events[0]);
    return(0);
}
