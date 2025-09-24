/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:02:53 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/24 17:52:23 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <string>
#include <unistd.h>

#define BUFFER_SIZE 512

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

int init(sockaddr_in hote, int fd, int port)
{
    hote.sin_family = AF_INET;
    hote.sin_port = htons(port);
    hote.sin_addr.s_addr = htonl(INADDR_ANY);
    clearsinzero(&hote);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, reinterpret_cast<struct sockaddr *>(&hote), sizeof(hote)) == -1)
    {
        close(fd);
        throw(std::runtime_error("ERR_CONNECT"));
    }
}

void sendToServer(int fd, std::string msg)
{
    msg += "\r\n";
    send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
}
int main(int ac, char **av)
{
    if (ac != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
    std::string ip = av[1];
    int port = range_port(av[1]);
    int fd = -1;
    sockaddr_in hote;
    try
    {
        init(hote, fd, port);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    std::string msg = "PASS " + static_cast<std::string>(av[2]);
    sendToServer(fd, msg);
    sendToServer(fd, "NICK WALL-E");
    sendToServer(fd, "USER WALL-E WALL-E 0 :IM WALL-E THE BOT");
}