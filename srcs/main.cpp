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

int create_server(Server& Server, int port, char *password) // mettre le truc de reference.
{
    int epfd;
    epfd = epoll_create1(0);
    Server.password = password;

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

int wait_client(Server Server)
{
    int nfds;
    int nbrclient;

    while(!stop)
    {
        nfds = epoll_wait(Server.epfd, Server.events, MAX_EVENTS , -1); // -1 ?
        if (nfds == -1) // que faire ?
            Server.Finish();
        for (int i = 0; i < nfds ; ++i)
        {
            if (nfds && Server.events[i].data.fd == Server.fd) // new client
            {
                Client* Clients = new Client(Server.password);
                if (Clients->Init(Server.epfd, Server.fd))
                {
                    delete Clients;
                    std::cerr << "Failed to accept new client" << std::endl;
                    continue;
                }
                Server.client_list.push_back(Clients);
                std::cout << "New client connected\r\n" << std::endl;
            }
            else if (nfds && Server.events[i].data.fd != Server.fd)
            {
                nbrclient = find_client(Server.client_list, Server.events[i].data.fd);
                if (Server.events[i].events == EPOLLIN)
                    Server.client_list[nbrclient]->ReadMsg(nbrclient);
                else if (Server.events[i].events == EPOLLOUT) 
                    Server.client_list[nbrclient]->PushMsg("MON MSG");
                else if (Server.events[i].events == EPOLLHUP)
                    Server->Close_client(nbrclient, "HUP"); // change le big 3, par une fonction dans le serveur qui nettoie tout.
                else if (Server.events[i].events == EPOLLRDHUP)
                    Server->Close_client(nbrclient, "RDHUP");
                else if (Server.events[i].events == EPOLLERR)
                    Server->Close_client(nbrclient, "ERR"); // change le big 3, par une fonction dans le serveur qui nettoie tout.
            }
        }
    }
    return 0;
}

int range_port(int port)
{
    const char *s = port;
    char *end;
    errno = 0;
    long val = strtol(s, &end, 10);
    if (errno == ERANGE || val > 65535 || val <= 0)
        std::cerr << "Port number out of range." << std::endl;
    if (*end != '\0')
        std::cerr << "Invalid port number." << std::endl;
    int n = static_cast<int>(val);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    signal(SIGINT, &handler);
    Server Server();

    range_port(argv[1]); // a mettre dans le try and catch .
    
    try
    {
        create_server(Server, n, argv[2]);
        wait_client(Server);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}