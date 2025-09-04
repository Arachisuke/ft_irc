/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/03 16:50:10 by ankammer         ###   ########.fr       */
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
class Channel;


class Client
{

public:
    Client();
    ~Client();
    void Send_Welcome();
    int Init(int epfd, int hote);

    void setNickname(std::string nickName);
    void setUsername(std::string userName);
    void setMode(std::string mode);
    std::string & setBuffer();
    // void setListOfChannel(std::string channelName);
    void setFd(int fd);
    void setPasswordStatus(int passwordStatus);
    void setNicknameStatus(int nickNameStatus);
    void setUsernameStatus(int userNameStatus);
    void setIsRegistered(int isRegistered);
    std::vector<Channel *>& setMyChannel();


    const std::string getPrefiks() const;
    const std::string getNickname() const;
    const std::string getUsername() const;
    const std::string getMode() const;
    const std::string getBuffer() const;
    const std::vector<Channel *> &getMyChannel() const;
    int getFd() const;
    int getPassword_Status() const;
    int getNickname_Status() const;
    int getUsername_Status() const;
    int getisRegistered() const;

private:
    int _fd;
    std::vector<Channel *> _myChannels;
    int _Password_Status;
    int _Nickname_Status;
    int _Username_Status;
    int _isRegistered;
    std::string _nickname; // 9 length
    std::string _username; // 9 length
    std::string _mode;     // 3 length
    std::string _buffer;
    int _hote;
    struct epoll_event _event;
    struct sockaddr_in _client;
    socklen_t _size_of_client;
};
#endif