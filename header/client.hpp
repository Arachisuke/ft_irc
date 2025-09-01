/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 16:44:47 by wzeraig          ###   ########.fr       */
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
#define ERR_NEEDMOREPARAMS "Not Enough Parameters"
#define ERR_ALREADYREGISTRED "You may not reregister"
#define ERR_PASSWDMISMATCH "Password Incorrect"
#define ERR_ERRONEUSNICKNAME "Erroneus Nickname"
#define ERR_NICKNAMEINUSE "Nickname is already in use"
#define ERR_NONICKNAMEGIVEN "No nickname given"
#define ERR_TOOMANYARGS "Too many arguments"
#define ERR_UNKNOWNCOMMAND "Unknown command"
#define ERR_NOSUCHNICK "No such nick/channel"
#define ERR_NOSUCHCHANNEL "No such channel"
#define ERR_CANNOTSENDTOCHAN "Cannot send to channel"
#define ERR_TOOMANYCHANNELS "Too many channels"
#define ERR_WASNOSUCHNICK "There was no such nickname"

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
    int hote;
    struct epoll_event event;
    struct sockaddr_in client;
    socklen_t size_of_client;
};
#endif