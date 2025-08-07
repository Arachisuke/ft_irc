/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:06:34 by macos             #+#    #+#             */
/*   Updated: 2025/08/07 17:05:05 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include <string>

void clearsinzero(struct sockaddr_in *addr)
{
    for (int i = 0; i < 8; ++i)
        addr->sin_zero[i] = 0;
}
void translationclient_to_irc(std::string& s) {
    if (!s.empty() && s.back() == '\n') s.pop_back();  // retire \n
    if (!s.empty() && s.back() == '\r') s.pop_back();  // retire \r
}

std::string lecture(int client, int fd, int *bytes) //lit en str, transforme en std::string, clarifie la string, la renvoie
{
    char lecture[512];
    
    *bytes = recv(client, &lecture, sizeof(lecture), MSG_NOSIGNAL);
    if (*bytes == -1)
    {
        std::cout << "ERR_READ" << std::endl;
        close(client);
        close(fd);
        return(NULL);
    }
    else if (*bytes > 0)
    {
        std::string message(lecture, *bytes);
        translationclient_to_irc(message); // necessite une protection si le *bytes = 0.
        return(message);
    }
    else
        return(NULL);
}

void PushMsg(int client, std::string msg, int special)
{
    (void)special; // je me le reserve pour apres
    msg.push_back('\r'); 
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(client, msg.c_str(), msg.size(), MSG_DONTWAIT);
}
int create_server()
{
    struct sockaddr_in hote;
    int fd;
    hote.sin_family = AF_INET;
    hote.sin_port = htons(6667);
    hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&hote);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        return(std::cout << "ERR_FD" << std::endl, 50);
    //fcntl(fd, F_SETFL, O_NONBLOCK); // Rendre non bloquant
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
        return(50);
    if (bind(fd, reinterpret_cast<struct sockaddr*>(&hote), sizeof(hote)) == -1)
        return(std::cout << "ERR_BIND" << std::endl, 50);
    if(listen(fd, 10) == -1)
        return(std::cout << "ERR_LISTEN" << std::endl, 50);
    return fd;
}
int wait_client(int hote)
{
    int fd;
    struct sockaddr_in client;
    socklen_t size_of_client;
    while(1)
    {
        fd = accept(hote, reinterpret_cast<struct sockaddr*>(&client), &size_of_client); // tester sans la boucle, peut etre que accept je reste dessus tant que j'ai pas de demande. ou c'est listen qui est passif et qui attend.
        if (fd == -1)
            return(std::cout << "ERR_ACCEPT" << std::endl ,close(fd), close(hote), 50);
        break;
    }
    return fd;
}
int main(int argc, char **argv)
{
    if (argc != 3) 
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
    
    std::string password = argv[2]; 
    std::string entry;
    std::string nickname; // 9 length a stocker dans une class client
    std::string username; // 9 length a stocker dans une class client 
    
    int bytes;
    int client_fd;
    int fd;
    
    fd = create_server();
    if (fd == 50)
        return 1;
    client_fd = wait_client(fd);
    if (client_fd == 50)
        return 1;
   
    entry = lecture(client_fd, fd, &bytes); // if bytes == 0 ?
    if (bytes == -1)
        return(1);
        
    if (entry != password)
        return(std::cout << "Wrong Password" << std::endl ,close(client_fd), close(fd), 1);
        
    nickname = lecture(client_fd, fd, &bytes);
    if (bytes == -1)
        return(1);
        
    username = lecture(client_fd, fd, &bytes);
    if (bytes == -1)
        return(1);
        
    std::string welcome_msg =
    ":Hueco Mundo 001 " + nickname +
    " :Welcome to the Hueco Mundo Network, " + nickname + "!~" + username + "@localhost\r\n";
    send(client_fd, welcome_msg.c_str(), welcome_msg.size(), 0);
    
    while(1)
    {
        entry = lecture(client_fd, fd, &bytes);
        if (entry == "exit")
            return(std::cout << "EXIT" << std::endl ,close(client_fd), close(fd), 0);
        PushMsg(client_fd, "tu as ecris : ", 0);
        PushMsg(client_fd, entry, 0);
    }
    close(client_fd);
    close(fd);
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