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

#include "../header/client.hpp"
#include "../header/server.hpp"

int create_server(class Server& Server) // mettre le truc de reference.
{
    int epfd;
    epfd = epoll_create1(0);
    if (Server.Init(epfd)) // gerer les erreurs.
        return(1);
    return 0;
}

int find_client(std::vector<Client> client_list, int fd)
{
    if (client_list.empty())
        return (-1);
    for (size_t i = 0; i < client_list.size() - 1; i++)
    {
        std::cout << client_list[i].fd << std::endl;
        std::cout << fd << std::endl;
       if  (client_list[i].fd == fd)
            return(i);
    }
    return(-1);
}

int wait_client(class Server Server, std::vector<Client> client_list)
{
    int nfds; // nombre de fd en Server.events
    int nbrclient;

    while(1)
    {
        nfds = epoll_wait(Server.epfd, Server.events, 5, 0);
        if (nfds == -1)
            return(std::cout << "EPOLL WAIT", 50); // a verifier si c'est la bonne erreur a renvoyer
        for (int i = 0; i < nfds ; ++i)
        {


            std::cout << "fd du server : " << Server.fd << std::endl;
            std::cout << "fdreactionnel : " << Server.events[i].data.fd << std::endl;
            
            if (Server.events[i].data.fd == Server.fd) // newclientconnexion cree une class Clients
            {
                Client Clients;
               if (Clients.Init(Server.epfd, Server.fd))
                   return(std::cout << "Accept" << std::endl, 1);
                if (Clients.Registration())
                   return(std::cout << "Wrong Password" << std::endl, 1);
                client_list.push_back(Clients);
            }
            else
            {
                nbrclient = find_client(client_list, Server.events[i].data.fd);
                if (nbrclient == -1)
                {
                    std::cout << "ERR_FIND_CLIENT" << std::endl;
                    continue;
                }
                if (Server.events[i].events == EPOLLIN)
                    client_list[nbrclient].ReadMsg();
                if (Server.events[i].events == EPOLLOUT) // comment marche une discussion server/client 
                {
                    client_list[nbrclient].PushMsg("MON MSG");
                    Server.events[i].events = EPOLLIN;
                    epoll_ctl(Server.epfd, EPOLL_CTL_MOD, Server.events[i].data.fd,  &Server.events[i]); // CTLMOD.
                }
                if (Server.events[i].events == EPOLLHUP) // deconnexion
                    client_list.erase(client_list.begin() + nbrclient);
                if (Server.events[i].events == EPOLLERR) // error, pas encore implementer
                {
                    std::cout << "ERROR\r" << std::endl;
                    client_list.erase(client_list.begin() + nbrclient);
                }
            }
        }
    }
    return 0;
}
int main(int argc, char **argv)
{
    if (argc != 3) 
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
    std::vector<Client> client_list; // vector de class clients.
    Server Server;
    Server.password = argv[2];

    if (std::atoi(argv[1]) < 1024 || std::atoi(argv[1]) > 65535)
        return (std::cerr << "Port must be between 1024 and 65535" << std::endl, 1);
    if (create_server(Server)) // gerer les erreurs
        return(1);
    
    wait_client(Server, client_list); // gerer les erreurs
    
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