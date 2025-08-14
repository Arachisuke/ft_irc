/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:35:30 by macos             #+#    #+#             */
/*   Updated: 2025/08/14 13:09:00 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/server.hpp"
#include "../header/client.hpp"

bool stop = false;

void handler(int) { stop = true; }

int create_server(Server& Server, int port) // mettre le truc de reference.
{
    int epfd;
    epfd = epoll_create1(0);
    if (Server.Init(epfd, port)) // gerer les erreurs.
        return(-1);
    return 0;
}

int find_client(std::vector<Client*> client_list, int fd)
{
    if (client_list.empty())
        return (-1);
    for (size_t i = 0; i < client_list.size(); i++)
    {
       if  (client_list[i]->fd == fd)
            return(i);
    }
    return(-1);
}

int wait_client(Server Server, std::vector<Client*> client_list)
{
    int nfds;
    int nbrclient;

    while(!stop)
    {
        nfds = epoll_wait(Server.epfd, Server.events, 5, -1);
        if (nfds == -1) // que faire ?
            Server.Finish();
        for (int i = 0; i < nfds ; ++i)
        {
            if (nfds && Server.events[i].data.fd == Server.fd) // new client
            {
                Client* Clients = new Client(client_list, Server.password);
                if (Clients->Init(Server.epfd, Server.fd))
                {
                    Clients->Big_3(client_list, nbrclient, "ERR_ACCEPT");
                    continue;
                }
                client_list.push_back(Clients);
                std::cout << "New client connected\r\n" << std::endl;
            }
            else if (nfds && Server.events[i].data.fd != Server.fd)
            {
                nbrclient = find_client(client_list, Server.events[i].data.fd);
                if (Server.events[i].events == EPOLLIN)
                    client_list[nbrclient]->ReadMsg(nbrclient);
                else if (Server.events[i].events == EPOLLOUT) 
                    client_list[nbrclient]->PushMsg("MON MSG");
                else if (Server.events[i].events == EPOLLHUP)
                    client_list[nbrclient]->Big_3(client_list, nbrclient, "HUP");

                else if (Server.events[i].events == EPOLLERR)
                    client_list[nbrclient]->Big_3(client_list, nbrclient, "ERR");
            }
        }
    }
    return 0;
}
int main(int argc, char **argv)
{
    if (argc != 3) 
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
    
    signal(SIGINT, &handler); // implementation pas termine.
    std::vector<Client*> client_list; 
    Server Server(client_list);
    Server.password = argv[2];

    std::string str = argv[1];
    const char* s = str.c_str();
    char* end;
    errno = 0;
    long val = strtol(s, &end, 10);
    if (errno == ERANGE || val > 65535 || val <= 0)
        std::cerr << "Port number out of range." << std::endl;
    if (*end != '\0')
        std::cerr << "Invalid port number." << std::endl;
    int n = static_cast<int>(val);

    if (create_server(Server, n))
        return(1);
    
    if (wait_client(Server, client_list))
        return(1);
    return 0;
}