/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:35:30 by macos             #+#    #+#             */
/*   Updated: 2025/08/11 15:45:39 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "../header/client.hpp"
#include "../header/server.hpp"
#include <string>

// deplacement de sinzero dans la class Server

void translationclient_to_irc(std::string &s)
{
    if (!s.empty() && s[s.size() - 1] == '\n')
        s.erase(s.size() - 1, 1);
    if (!s.empty() && s[s.size() - 1] == '\r')
        s.erase(s.size() - 1, 1);
}

std::string lecture(int client, int *bytes) // lit en str, transforme en std::string, clarifie la string, la renvoie
{
    char lecture[512];

    *bytes = recv(client, &lecture, sizeof(lecture), MSG_NOSIGNAL);
    if (*bytes == -1)
    {
        std::cout << "ERR_READ" << std::endl;
        close(client);
        return (NULL);
    }
    else if (*bytes > 0)
    {
        std::string message(lecture, *bytes);
        translationclient_to_irc(message); // necessite une protection si le *bytes = 0.
        return (message);
    }
    else
        return (NULL);
}

void PushMsg(int client, std::string msg, int special) // 2 pushMsg un dans le main et un dans la class Client normal?
{
    if (!this->RPL_WELCOME)
        return (this->Send_Welcome(), (void)0);
    // fonction avec des booleans, ex : RPLWELCOME a 0 ? ca veut dire je dois envoyer RPLWELCOME. ne pas oublier de le mettre en EPOLLIN apres.
    (void)special; // je me le reserve pour apres
    msg.push_back('\r');
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(client, msg.c_str(), msg.size(), MSG_DONTWAIT);
}
int create_server(class Server &Server) // mettre le truc de reference.
{
    int epfd;
    int yes = 1;

    epfd = epoll_create1(0);
    Server.Init(epfd); // gerer les erreurs.

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        return (50);
    if (bind(fd, reinterpret_cast<struct sockaddr *>(&hote), sizeof(hote)) == -1)
        return (std::cout << "ERR_BIND" << std::endl, 50);
    if (listen(fd, 10) == -1)
        return (std::cout << "ERR_LISTEN" << std::endl, 50);
    return fd;
}
Client &find_client(std::vector<Client> client_list, int fd) // correction Clients en Client
{
    for (size_t i = 0; i < client_list.size() - 1; i++)
    {
        if (client_list[i].fd = fd)
            return (client_list[i]);
    }
    return (NULL);
}

int wait_client(Server server, std::vector<Client> client_list) // correction Clients en Client et modification class Server -> Server server
{
    int nfds; // nombre de fd en event

    while (1)
    {
        nfds = epoll_wait(fd, events, 5, 0); // fd -> server.events et events -> server.events ?
        if (nfds == -1)
            return (std::cout << "EPOLL WAIT", 50); // a verifier si c'est la bonne erreur a renvoyer
        Client Clients; // deplacement variable 
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == hote) // newclientconnexion cree une class Clients // hote -> server.hote?
            {
                if (Clients.Init(hote))
                    return (std::cout << "Accept" << std::endl, 1);
                if (Clients.Registration(hote))
                    return (std::cout << "Wrong Password" << std::endl, 1);
                client_list.push_back(Clients);
            }
            else
            {
                Clients = find_client(client_list, events[i].data.fd); // correction event -> events
                if (events[i].events == EPOLLIN)
                    Clients.ReadMsg();
                if (events[i].events == EPOLLOUT) // comment marche une discussion server/client
                {
                    Clients.PushMsg(events[i].data.fd, "MON MSG", &bytes); // parametre push msg du main mais utilise l'implementation de la class et correction event -> events
                    events[i].events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, events); // CTLMOD. // server.epfd -> epfd?
                }
                if (events[i].events == EPOLLHUP) // deconnexion
                    delete Clients;               // stack pourquoi delete ?
                if (events[i].events == EPOLLERR) // error, pas encore implementer
                {
                    std::cout << "ERROR\r" << std::endl;
                    delete Clients; // stack pourquoi delete ?
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
    std::vector<Client> client_list; // vector de class clients.
    class Server Server;

    create_server(Server); // gerer les erreurs

    wait_client(Server, client_list); // gerer les erreurs

    delete Server; // stack pourquoi delete ?
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