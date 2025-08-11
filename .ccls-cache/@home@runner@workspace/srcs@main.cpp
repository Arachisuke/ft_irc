/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:35:30 by macos             #+#    #+#             */
/*   Updated: 2025/08/11 08:07:36 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/client.hpp"
#include "../header/server.hpp"

int create_server(class Server& Server) // mettre le truc de reference.
{
    int epfd;
    int yes = 1;

    epfd = epoll_create1(0);
    Server.Init(epfd); // gerer les erreurs.
    
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
        return(50);
    if (bind(fd, reinterpret_cast<struct sockaddr*>(&hote), sizeof(hote)) == -1)
        return(std::cout << "ERR_BIND" << std::endl, 50);
    if(listen(fd, 10) == -1)
        return(std::cout << "ERR_LISTEN" << std::endl, 50);
    return fd;
}
Client& find_client(std::vector<Clients> client_list, int fd)
{
    for (size_t i = 0; i < client_list.size() - 1; i++)
    {
       if  (client_list[i].fd = fd)
            return(client_list[i]);
    }
    return(NULL);
}

int wait_client(class Server, std::vector<Clients> client_list)
{
    int nfds; // nombre de fd en event
        
    while(1)
    {
        nfds = epoll_wait(fd, events, 5, 0); 
        if (nfds == -1)
            return(std::cout << "EPOLL WAIT", 50); // a verifier si c'est la bonne erreur a renvoyer
        for (int i = 0; i < nfds ; ++i)
        {
            if (events[i].data.fd == hote) // newclientconnexion cree une class Clients
            {
                Client Clients;
               if (Clients.Init(hote))
                   return(std::cout << "Accept" << std::endl, 1);
                if (Clients.Registration(hote))
                   return(std::cout << "Wrong Password" << std::endl, 1);
                client_list.push_back(Clients);
            }
            else
            {
                Clients = find_client(client_list, event[i].data.fd);
                if (events[i].events == EPOLLIN)
                    Clients->ReadMsg();
                if (events[i].events == EPOLLOUT) // comment marche une discussion server/client 
                {
                    Clients->PushMsg(event[i].data.fd, "MON MSG", &bytes);
                    events[i].events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd,  events); // CTLMOD.
                }
                if (events[i].events == EPOLLHUP) // deconnexion
                    delete Clients;
                if (events[i].events == EPOLLERR) // error, pas encore implementer
                {
                    std::cout << "ERROR\r" << std::endl;
                    delete Clients;
                }
            }
            
        }
        
    }
    return fd;
}
int main(int argc, char **argv)
{
    if (argc != 3) 
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
    std::vector<Clients> client_list; // vector de class clients.
    class Server Server;
    
    create_server(Server); // gerer les erreurs
    
    wait_client(Server, client_list); // gerer les erreurs
           
    
        
    delete Server; 
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