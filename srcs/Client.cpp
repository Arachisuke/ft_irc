/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:36:21 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/23 16:43:56 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Server.hpp"
#include "../header/Client.hpp"

Client::Client()
{
    this->_isRegistered = 0;
    this->_PasswordStatus = 0;
    this->_NicknameStatus = 0;
    this->_realNameStatus = 0;
    this->_isRegistered = 0;
    this->_fd = -1;
    this->_isIrssi = 0;
}
Client::~Client()
{
    std::cout << "Client disconnected\r\n"
              << std::endl;
}

int Client::Init(int epfd, int hote)
{
    this->_size_of_client = sizeof(this->_client);
    this->_hote = hote;

    this->_fd = accept(this->_hote, reinterpret_cast<sockaddr *>(&this->_client), &_size_of_client);
    if (_fd == -1)
        return (1);
    this->_event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->_event.data.fd = this->_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, this->_fd, &this->_event);
    return (0);
}

const std::string Client::getPrefiksClient() const
{
    return (":" + _nickname + "!" + _realName + "@localhost");
}

const std::string &Client::getNickname() const
{
    return (_nickname);
}
const std::string &Client::getUsername() const
{
    return (_realName);
}
const std::string &Client::getMode() const
{
    return (_mode);
}
const std::string &Client::getBuffer() const
{
    return (_buffer);
}

void Client::setNickname(std::string nickName)
{
    _nickname = nickName;
}
void Client::setUsername(std::string userName)
{
    _realName = userName;
}
void Client::setMode(std::string mode)
{
    _mode = mode;
}
std::string &Client::setBuffer()
{
    return (_buffer);
}

int Client::getFd() const
{
    return (_fd);
}
const std::vector<Channel *> &Client::getMyChannel() const
{
    return (_myChannels);
}
int Client::getPassword_Status() const
{
    return (_PasswordStatus);
}
int Client::getNickname_Status() const
{
    return (_NicknameStatus);
}

const std::string & Client::getRealname() const
{
    return (_realName);
}

int Client::getUsername_Status() const
{
    return (_realNameStatus);
}
int Client::getisRegistered() const
{
    return (_isRegistered);
}
int Client::getIrssi() const
{
    return (_isIrssi);
}


std::vector<Channel *> &Client::setMyChannel()
{
    return (_myChannels);
}
void Client::setFd(int fd)
{
    _fd = fd;
}
void Client::setIrssi(int irssi)
{
    _isIrssi = irssi;
}
void Client::setPasswordStatus(int passwordStatus)
{
    _PasswordStatus = passwordStatus;
}
void Client::setNicknameStatus(int nickNameStatus)
{
    _NicknameStatus = nickNameStatus;
}
void Client::setUsernameStatus(int userNameStatus)
{
    _realNameStatus = userNameStatus;
}
void Client::setIsRegistered(int isRegistered)
{
    _isRegistered = isRegistered;
}
void Client::removeMyChannel(Channel *channelToRemove)
{
    if (std::find(_myChannels.begin(), _myChannels.end(), channelToRemove) != _myChannels.end())
        this->_myChannels.erase(std::find(_myChannels.begin(), _myChannels.end(), channelToRemove));
}