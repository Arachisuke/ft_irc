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

Server::Server( std::vector<Client*>& client_list) : server_to_client_list(client_list)
{}

void Server::close_all_client()
{
    for (size_t i = 0; i < this->server_to_client_list.size(); i++)
    {
        if (this->server_to_client_list[i] && this->server_to_client_list[i]->fd != -1)
            close(this->server_to_client_list[i]->fd);
        delete this->server_to_client_list[i];
    }
}
Server::~Server() {
    
    this->close_all_client();
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
        return(std::cout << "ERR_FD" << std::endl, -1);
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
        return(close(this->fd), 50);

    if (bind(this->fd, reinterpret_cast<struct sockaddr*>(&this->hote), sizeof(this->hote)) == -1)
        return(close(this->fd),  std::cout << "ERR_BIND" << std::endl, -1);

    if(listen(this->fd, 10) == -1)
        return(close(this->fd), std::cout << "ERR_LISTEN" << std::endl, -1);
    
    fcntl(fd, F_SETFL, O_NONBLOCK); 
    this->epfd = epfd;
    this->events[0].events = EPOLLIN;
    this->events[0].data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->events[0]);
    return(0);
}
