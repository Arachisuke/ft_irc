/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:02:53 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/29 14:21:32 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <csignal>
#include <map>
#include <iostream>
#include <sstream>

#define LECTURE_SIZE 512

bool stop = false;

void handler(int)
{
    stop = true;
}

int range_port(char *port)
{
    const char *s = port;
    char *end;
    long val;

    errno = 0;
    val = strtol(s, &end, 10);
    if (errno == ERANGE || val > 65535 || val <= 0)
        return (std::cerr << "Port number out of range." << std::endl, -1);
    if (*end != '\0')
        return (std::cerr << "Invalid port number." << std::endl, -1);
    return (static_cast<int>(val));
}
void clearsinzero(struct sockaddr_in *addr)

{
    for (int i = 0; i < 8; ++i)
        addr->sin_zero[i] = 0;
}

int init(int port)
{
    int fd;
    sockaddr_in hote;
    hote.sin_family = AF_INET;
    hote.sin_port = htons(port);
    hote.sin_addr.s_addr = inet_addr("127.0.0.1");
    clearsinzero(&hote);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, reinterpret_cast<struct sockaddr *>(&hote), sizeof(hote)) == -1)
    {
        close(fd);
        throw(std::runtime_error("ERR_CONNECT"));
    }
    return (fd);
}

void sendToServer(int fd, std::string msg)
{
    msg += "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

int extractLine(int bytes, int fd, char *lecture, std::string &entry, std::string &buffer)
{
    size_t pos;
    if (bytes == -1) // ca ne peut pas arrive.. vu que c'est le server qui lui envoie des msg
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (1);
        std::cerr << "ERR_READ" << std::endl;
        return (close(fd), 1);
    }
    // else if (bytes == 0) // controle C dans le server. + tout le temps avec le dontwait.
    //     return (close(fd), 1);
    else if (bytes > 0)
    {
        buffer.append(lecture, bytes);
        pos = buffer.find("\r\n");
        while (pos != std::string::npos)
        {
            entry = buffer.substr(0, pos);
            buffer.erase(0, pos + 2);
            pos = buffer.find("\r\n");
        }
        return (0);
    }
    return (0);
}

void sendMsg(std::string entry, int fd) // proteger le nickname et le msg ""
{
    if (stop)
        return;
    std::string nickname;
    size_t pos = entry.find("!");
    nickname = entry.substr(1, pos - 1);
    std::cout << "NICKNAME : " << nickname << std::endl;
    entry = entry.substr(1);
    pos = entry.find(":");
    std::string prvmsg = "PRIVMSG ";

    int randomValue = rand() % 5;

    std::cout << "value : " << randomValue << std::endl;

    std::vector<std::string> messages(5);
    messages[0] = " Le saviez-vous ? Les pieuvres ont trois cœurs.";
    messages[1] = " Le saviez-vous ? Le miel ne se périme jamais.";
    messages[2] = " Le saviez-vous ? Les girafes dorment moins de deux heures par jour.";
    messages[3] = " Le saviez-vous ? La Terre n'est pas parfaitement ronde.";
    messages[4] = " Le saviez-vous ? Les papillons goûtent avec leurs pattes.";

    std::string msg = prvmsg + nickname + messages[randomValue] + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

int main(int ac, char **av)
{
    if (ac != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    signal(SIGINT, &handler);
    signal(SIGQUIT, &handler);
    srand(time(NULL));

    std::string ip = av[1];
    int port = range_port(av[1]);
    int fd = -1;
    try
    {
        fd = init(port);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    std::string msg = "PASS " + static_cast<std::string>(av[2]);

    sendToServer(fd, msg);
    sendToServer(fd, "NICK WALL-E");
    sendToServer(fd, "USER WALL-E WALL-E 0 :IM WALL-E THE BOT");

    int bytes;
    char lecture[LECTURE_SIZE];
    std::string entry;
    std::string buffer;
    while (!stop)
    {
        bytes = recv(fd, &lecture, LECTURE_SIZE, MSG_DONTWAIT);
        if (extractLine(bytes, fd, lecture, entry, buffer) == 0)
        {
            if (entry.find("PRIVMSG WALL-E") != std::string::npos)
                sendMsg(entry, fd);
        }
        // si c'est 0, ca veut dire controle C dans le server, mais vu que je dontwait il aura souvent des 0. je dontwait car il reagissait pas au controle C dans le bot.
      
    }
    close(fd);
    return (0);
}

// lancer double bot @@@@
// controle C reaction @@@@@
// controle C server -> on ne reagis pas @@@@
// same name -> pas besoin de gerer en vrai @@@
// test de same name en temps reel. @@@@
