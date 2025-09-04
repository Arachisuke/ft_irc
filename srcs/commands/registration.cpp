/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:57 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/04 17:41:06 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

void Server::pass()
{
    if (this->_cmd.size() - 1 == 0)
        return (reply(461, "PASS", "Not enough parameters", *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 1)
        return (reply(462, "PASS", "You may not reregister", *this->_clientList[this->_nbrclient]), (void)0);
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
    std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost NICK : " + this->_cmd[1] + "\r\n";
    for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++)
    {
        const std::set<Client *> &users = this->_clientList[this->_nbrclient]->getMyChannel()[i]->getUsers();
        for (std::set<Client *>::const_iterator it = users.begin(); it != users.end(); ++it)
        {
            if ((*it)->getFd() != this->_clientList[this->_nbrclient]->getFd())
                send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
        }
    }
    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
}

void Server::nick()
{
    if (this->_cmd.size() - 1 == 0)
        return (reply(461, "NICK", "Not enough parameters", *this->_clientList[this->_nbrclient]), (void)0);
    if (!this->_clientList[this->_nbrclient]->getPassword_Status())
        return (reply(461, "NICK", "ERR_PASSWORDBEFORE", *this->_clientList[this->_nbrclient]), (void)0); // je renvoie quoi finalement ?
    if (nickpolicy())
        return (reply(432, "NICK", "Erroneus nickname", *this->_clientList[this->_nbrclient]), (void)0); // erroneus nickname
    if (findNick())
        return (reply(433, "NICK", "Nickname is already in use", *this->_clientList[this->_nbrclient]), (void)0); // nickname in use
    this->successfullNick();                                                                                      // si le mdp est faux j'envoie quand meme un successful.
    this->_clientList[this->_nbrclient]->setNickname(this->_cmd[1]);
    this->_clientList[this->_nbrclient]->setNicknameStatus(1);
    return;
}

void Server::user()
{
    if (this->_cmd.size() - 1 < 4)
        return (reply(461, "USER", "Not enough parameters", *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getUsername_Status() == 0)
    {
        if (!this->_clientList[this->_nbrclient]->getPassword_Status() || !this->_clientList[this->_nbrclient]->getNickname_Status())
            return (std::cout << "no mais allo\n", reply(461, "USER", "ERR_NEEDPASSWORDORNICK", *this->_clientList[this->_nbrclient]), (void)0); // je renvoie quoi finalement ?
        // is print ?
        if (this->_cmd[4].size() > 9 || this->_cmd[4].size() < 1)
            return (reply(432, "USER", "Erroneus nickname", *this->_clientList[this->_nbrclient]), (void)0);

        if (this->_clientList[this->_nbrclient]->getPassword_Status() == -1)
            return this->closeClient("ERR_PASSWDMISMATCH");
        this->_clientList[this->_nbrclient]->setUsername(this->_cmd[1]);
        this->_clientList[this->_nbrclient]->setUsernameStatus(1);
        this->_clientList[this->_nbrclient]->setIsRegistered(1);
        std::string welcome_msg =
            ":Hueco Mundo 001 " + this->_clientList[this->_nbrclient]->getNickname() +
            " :Welcome to the Hueco Mundo Network, " + this->_clientList[this->_nbrclient]->getNickname() + "!~" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost\r\n";
        send(this->_clientList[this->_nbrclient]->getFd(), welcome_msg.c_str(), welcome_msg.size(), MSG_DONTWAIT);
    }
    else
        return (reply(462, "USER", "You may not reregister", *this->_clientList[this->_nbrclient]), (void)0);
    // join j'ai enlever le epollout. etc etc
}