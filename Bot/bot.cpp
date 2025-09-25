/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:02:53 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/25 16:39:30 by ankammer         ###   ########.fr       */
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

#define LECTURE_SIZE 512

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
    if (bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (1);
        std::cerr << "ERR_READ" << std::endl;
        return (close(fd), 1);
    }
    else if (bytes == 0)
    {
        std::cerr << "ERR_READ" << std::endl;
        return (close(fd), 1);
    }
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
    std::string nickname;
    size_t pos = entry.find("!");
    nickname = entry.substr(1, pos - 1);
    std::cout << "NICKNAME : " << nickname << std::endl;
    pos = entry.find(":");
    entry = entry.substr(1, pos);
    std::string prvmsg = "PRIVMSG";
    std::string msg = prvmsg + " tu as bien ecris ca ?\n" + entry + "\r\n";

    send(fd, msg.c_str(), msg.size(), 0);
}

int main(int ac, char **av)
{
    if (ac != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
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
    while (1)
    {
        bytes = recv(fd, &lecture, LECTURE_SIZE, 0);
        if (bytes <= 0)
        {
            std::cerr << "Connection closed by server" << std::endl;
            break;
        }

        if (extractLine(bytes, fd, lecture, entry, buffer) == 0)
        {
            if (entry.find("PRIVMSG WALL-E") != std::string::npos)
                sendMsg(entry, fd);
        }
    }
    close(fd);
    return (0);
}

// std::string privmsg = "REPLY ";
// std::string bot = ":bot!WALL-E:REPLY:";
// entry -> je lui repond par "  " via PRIVMSG.

// entry == find "bonjour" = 1
//  ncikname = entry(substr(1, "!")
//  msg = "ce que je veux"
//  send(fdbidirectionel, "PRIVMSG+nickname+ msg", size etc etc);
//  "PRIVMSG + MSG" => a analyser si ca marche
//  proteger ce que je recois, si c'est trop long ou non, par contre le retour pas besoin de le proteger car cmest moi je le controle
//