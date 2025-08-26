/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 14:58:39 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
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
#include "Server.hpp"

#define MAX_EVENTS 10
#define ERR_NEEDMOREPARAMS ":Not Enough Parameters\r\n"
#define ERR_ALREADYREGISTRED ":You may not reregister\r\n"
#define ERR_PASSWDMISMATCH ":Password Incorrect\r\n"
#define ERR_ERRONEUSNICKNAME ":Erroneus Nickname\r\n"
#define ERR_NICKNAMEINUSE ":Nickname is already in use\r\n"
#define ERR_NONICKNAMEGIVEN ":No nickname given\r\n"
#define ERR_TOOMANYARGS ":Too many arguments\r\n"
#define ERR_UNKNOWNCOMMAND ":Unknown command\r\n"
#define ERR_NOSUCHNICK ":No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL ":No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN ":Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS ":Too many channels\r\n"
#define ERR_WASNOSUCHNICK ":There was no such nickname\r\n"

class Server;

class Client
{

public:
    Client();
    ~Client();
    void Send_Welcome();
    int Init(int epfd, int hote);

    int fd;
    std::vector<std::string> listofchannel;
    int count_args();
    int RPL_WELCOME;
    int RPL_INFO;
    int Password_Status;
    int Nickname_Status;
    int Username_Status;
    int isRegistered;

    std::string nickname; // 9 length
    std::string username; // 9 length
    std::string mode;     // 3 length
    std::string buffer;

private:
    std::string translationclient_to_server(std::string message);
    int epfd;
    int hote;
    struct epoll_event event;
    struct sockaddr_in client;
    socklen_t size_of_client;
    std::string password;
};

#endif