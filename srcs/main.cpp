/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:35:30 by macos             #+#    #+#             */
/*   Updated: 2025/08/11 19:57:15 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/server.hpp"
#include "big_3.cpp"




int create_server(class Server& Server, int port) // mettre le truc de reference.
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



int wait_client(class Server Server, std::vector<Client*> client_list)
{
    int nfds;
    int nbrclient;

    while(1)
    {
        nfds = epoll_wait(Server.epfd, Server.events, 5, -1);
        if (nfds == -1)
            return(std::cout << "ERR_EPOLL_WAIT", -1); 
        for (int i = 0; i < nfds ; ++i)
        {
            if (nfds && Server.events[i].data.fd == Server.fd) // new client
            {
                Client* Clients = new Client(client_list, Server.password);
                if (Clients->Init(Server.epfd, Server.fd))
                    return(std::cout << "ERR_ACCEPT" << std::endl, delete Clients, 1); // vu que j'arrete a fd-1, je peux pas avoir de fd negatif. du coup pas ce cas a gerer.
                client_list.push_back(Clients);
                std::cout << "New client connected" << std::endl;
            }
            else if (nfds && Server.events[i].data.fd != Server.fd) // client already connected
            {
                nbrclient = find_client(client_list, Server.events[i].data.fd);
                if (nbrclient == -1)
                {
                    std::cout << "ERR_FIND_CLIENT" << std::endl;
                    continue;
                }
                for (int i = 0; i < nfds; i++)
                {
                    std::cout << "type d'event" << i << " : " << Server.events[i].events << std::endl;
                    std::cout << "FD reactionnel" << i << " : " << Server.events[i].data.fd << std::endl;
                    std::cout << "FD du client trouver" << i << " : " << client_list[nbrclient]->fd << std::endl;
                }
                if (Server.events[i].events == EPOLLIN)
                {
                   if (!client_list[nbrclient]->Username_Status)
                        client_list[nbrclient]->Registration(nbrclient); // Wrong password ... ?
                    else
                        client_list[nbrclient]->ReadMsg(nbrclient); // cmd.
                }
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

    std::vector<Client*> client_list; // vector de class clients.
    Server Server(client_list);
    Server.password = argv[2];

    std::string str = argv[1];
    const char* s = str.c_str();
    char* end;
    errno = 0;
    long val = strtol(s, &end, 10);
    if (errno == ERANGE || val > INT_MAX || val < INT_MIN)
        std::cerr << "Port number out of range." << std::endl;
    if (*end != '\0')
        std::cerr << "Invalid port number." << std::endl;
    int n = static_cast<int>(val);

    if (create_server(Server, n)) // gerer les erreurs
        return(1);
    
    if (wait_client(Server, client_list)) // gerer les erreurs
        return(1);
    return 0;
}

// fonction send + ajout de la norme
// fonction recve + suppression de la norme 
// les fonctions doivent etre aglomerer dans une autre grosse fonctions.
// je prend recve dans une fonction lecture, et dedans je recve + suppression et je renvoie un std::string ou pas en vrai ca depend si j'ai encore du traitement de texte a faire aprs ca.
// et pareil pour send.





// afficher PASS ? 
    // nickname ?? sortir un prompt pour poser la question ?
// gerer la fonction quit.