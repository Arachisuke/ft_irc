/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:57 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/30 16:36:25 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

void Server::pass()
{
    if (this->_cmd.size() - 1 == 0)
        return (reply(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 1)
        return (reply(462, this->_cmd[0], ERR_ALREADYREGISTRED, *this->_clientList[this->_nbrclient]), (void)0);
    this->_clientList[this->_nbrclient]->setPasswordStatus(1);
    if (this->_cmd[1] != this->_password)
        this->_clientList[this->_nbrclient]->setPasswordStatus(-1);
}

int Server::findNick()
{
    
    for (size_t i = 0; i < this->_clientList.size(); i++)
    {
        if (this->_clientList[i]->getNickname() == this->_cmd[1])
            return (1);
    }
    return (0);
}

int Server::nickpolicy()
{
    if (this->_cmd[1][0] == ':' || this->_cmd[1][0] == '#' || this->_cmd[1][0] == '&' || this->_cmd[1][0] == '!' || this->_cmd[1][0] == '@' || isdigit(this->_cmd[1][0]))
        return (1);
    for (size_t i = 0; i < this->_cmd[1].size(); i++)
    {
        if (isprint(this->_cmd[1][i]))
            return (1);
        if (this->_cmd[1][i] == ':')
            return (1);
    }
    if (this->_cmd[1].size() > 9)
        return (1);
    return (0);
}

int Server::isprint(char c)
{
    if (c < 33 || c > 126)
        return (1);
    return (0);
}

void Server::successfullNick()
{
    std::vector<int> alreadysent;
    std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost NICK " + this->_cmd[1] + "\r\n";
    for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++)
    {
        const std::set<Client *> &users = this->_clientList[this->_nbrclient]->getMyChannel()[i]->getUsers();
        for (std::set<Client *>::const_iterator it = users.begin(); it != users.end(); ++it)
        {
            if ((*it)->getFd() != this->_clientList[this->_nbrclient]->getFd() && std::find(alreadysent.begin(), alreadysent.end(), (*it)->getFd()) == alreadysent.end())
                send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
            alreadysent.push_back((*it)->getFd());
        }
    }
    if (this->_clientList[this->_nbrclient]->getUsername_Status() == 1)
        send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
}

void Server::nick()
{
    if (this->_cmd.size() - 1 == 0)
        return (reply(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *this->_clientList[this->_nbrclient]), (void)0);
    if (!this->_clientList[this->_nbrclient]->getPassword_Status())    if (this->_clientList[this->_nbrclient]->getUsername_Status() == 1)
        return (reply(464, this->_cmd[0], ERR_PASSWDMISMATCH, *this->_clientList[this->_nbrclient]), (void)0); 
    if (nickpolicy())
        return (reply(432, this->_cmd[0], ERR_ERRONEUSNICKNAME, *this->_clientList[this->_nbrclient]), (void)0);
    if (findNick())
        return (reply(433, this->_cmd[0], ERR_NICKNAMEINUSE, *this->_clientList[this->_nbrclient]), (void)0);
    this->successfullNick();
    this->_clientList[this->_nbrclient]->setNickname(this->_cmd[1]);
    this->_clientList[this->_nbrclient]->setNicknameStatus(1);
    return;
}

void Server::user()
{
    if (this->_cmd.size() - 1 < 4)
        return (reply(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getUsername_Status() == 0)
    {
        if (!this->_clientList[this->_nbrclient]->getPassword_Status() || !this->_clientList[this->_nbrclient]->getNickname_Status())
            return (reply(451, this->_cmd[0], ERR_NOTREGISTERED, *this->_clientList[this->_nbrclient]), (void)0); 
        if (this->_cmd[1].size() > 9 || this->_cmd[1].size() < 1)
            return (reply(432, this->_cmd[0], ERR_ERRONEUSNICKNAME, *this->_clientList[this->_nbrclient]), (void)0);

        if (this->_clientList[this->_nbrclient]->getPassword_Status() == -1)
            throw std::runtime_error(ERR_PASSWDMISMATCH);
        this->_clientList[this->_nbrclient]->setUsername(this->_cmd[1]);
        this->_clientList[this->_nbrclient]->setUsernameStatus(1);
        this->_clientList[this->_nbrclient]->setIsRegistered(1);
        Send_Welcome();
    }
    else
        return (reply(462, this->_cmd[0], ERR_ALREADYREGISTRED, *this->_clientList[this->_nbrclient]), (void)0);
}