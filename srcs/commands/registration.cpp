/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:57 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/01 15:01:53 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

void Server::pass()
{
    if (this->cmd.size() - 1 == 0)
        return(reply(461, "PASS", "Not enough parameters", *this->clientList[this->nbrclient]), (void)0);
    if (this->clientList[this->nbrclient]->isRegistered == 1)
    return(reply(462, "PASS", "You may not reregister", *this->clientList[this->nbrclient]), (void)0);
    this->clientList[this->nbrclient]->Password_Status = 1;
    if (this->cmd[1] != this->password)
        this->clientList[this->nbrclient]->Password_Status = -1;
}

int Server::findNick()
{
    for (size_t i = 0; i < this->clientList.size(); i++)
    {
        if (this->clientList[i]->nickname == this->entry)
            return (1);
    }
    return (0);
}

int Server::nickpolicy()
{
    if (this->cmd[1][0] == ':' || this->cmd[1][0] == '#' || this->cmd[1][0] == '&' || this->cmd[1][0] == '!' || this->cmd[1][0] == '@')
        return (1);
    for (size_t i = 0; i < this->cmd[1].size(); i++)
    {
        if (isprint(this->cmd[1][i]))
            return (1);
        if (this->cmd[1][i] == ':')
            return (1);
    }
    if (this->cmd[1].size() > 9)
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
    std::string msg = ":" + this->clientList[this->nbrclient]->nickname + "!" + this->clientList[this->nbrclient]->username + "@localhost NICK :" + this->cmd[1] + "\r\n";
    for (size_t i = 0; i < this->clientList.size(); i++)
        {
            if (this->clientList[i]->fd != this->clientList[this->nbrclient]->fd)
                send(this->clientList[i]->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
        }
}

void Server::nick() // toomanyarg ??
{
    if (this->cmd.size() - 1 == 0)
    return(reply(461, "NICK", "Not enough parameters", *this->clientList[this->nbrclient]), (void)0);
    if (!this->clientList[this->nbrclient]->Password_Status)
    return(reply(461, "NICK", "ERR_PASSWORDBEFORE", *this->clientList[this->nbrclient]), (void)0); // vrai erreur ?
    if (nickpolicy())
    return(reply(432, "NICK", "Erroneus nickname", *this->clientList[this->nbrclient]), (void)0); 
    if (findNick())
    return(reply(433, "NICK", "Nickname is already in use", *this->clientList[this->nbrclient]), (void)0); 
    this->clientList[this->nbrclient]->nickname = this->cmd[1];
    this->clientList[this->nbrclient]->Nickname_Status = 1;
    return;

}
void Server::user()
{
    if (this->cmd.size() - 1 < 4)
        return(reply(461, "USER", "Not enough parameters", *this->clientList[this->nbrclient]), (void)0);
    if (this->clientList[this->nbrclient]->Username_Status == 0) 
    {
        if (!this->clientList[this->nbrclient]->Password_Status || !this->clientList[this->nbrclient]->Nickname_Status)
        return(reply(461, "USER", "ERR_NEEDPASSWORDORNICK", *this->clientList[this->nbrclient]), (void)0); // je renvoie quoi finalement ?
        // is print ?ER 
        if (this->cmd[4].size() > 9 || this->cmd[4].size() < 1)
        return(reply(432, "USER", "Erroneus nickname", *this->clientList[this->nbrclient]), (void)0);

       
        if (this->clientList[this->nbrclient]->Password_Status == -1)
            return this->closeClient("ERR_PASSWDMISMATCH");
        this->clientList[this->nbrclient]->username = this->cmd[1];
        this->clientList[this->nbrclient]->Username_Status = 1;
        this->clientList[this->nbrclient]->isRegistered = 1;
        std::string welcome_msg =
            ":Hueco Mundo 001 " + this->clientList[this->nbrclient]->nickname +
            " :Welcome to the Hueco Mundo Network, " + this->clientList[this->nbrclient]->nickname + "!~" + this->clientList[this->nbrclient]->username + "@localhost\r\n";
        send(this->clientList[this->nbrclient]->fd, welcome_msg.c_str(), welcome_msg.size(), MSG_DONTWAIT);    
    }
    else
        return(reply(462, "USER", "You may not reregister", *this->clientList[this->nbrclient]), (void)0);
    // join j'ai enlever le epollout. etc etc
}