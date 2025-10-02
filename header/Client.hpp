/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/01 13:47:36 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;
class Channel;

class Client
{

public:
    Client();
    ~Client();

    int Init(int epfd, int hote);
    int getFd() const;
    int getPassword_Status() const;
    int getNickname_Status() const;
    const std::string &getRealname() const;
    int getUsername_Status() const;
    int getisRegistered() const;
    int getIrssi() const;


    void setNickname(std::string nickName);
    void setUsername(std::string userName);
    void setMode(std::string mode);
    void setFd(int fd);
    void setIrssi(int fd);
    void setPasswordStatus(int passwordStatus);
    void setNicknameStatus(int nickNameStatus);
    void setUsernameStatus(int userNameStatus);
    void setIsRegistered(int isRegistered);
    void removeMyChannel(Channel *channelToRemove);
    std::vector<Channel *> &setMyChannel();

    const std::string getPrefiksClient() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getMode() const;
    const std::string &getBuffer() const;
    const std::vector<Channel *> &getMyChannel() const;

    std::string &setBuffer();

private:
    int _hote;
    int _fd;
    int _PasswordStatus;
    int _NicknameStatus;
    int _realNameStatus;
    int _isRegistered;
    int _isIrssi;
    

    std::string _buffer;
    std::string _nickname; // 9 length
    std::string _realName;
    std::string _mode; // 3 length

    struct epoll_event _event;
    struct sockaddr_in _client;

    socklen_t _size_of_client;

    std::vector<Channel *> _myChannels;
};
#endif