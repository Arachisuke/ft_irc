/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:06 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/28 14:24:01 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <sstream>

Channel *Server::findChannelPtr(std::string &channelName)
{
    for (size_t i = 0; i < this->channeList.size(); i++)
    {
        if (this->channeList[i]->getName() == channelName)
            return (this->channeList[i]);
    }
    return (NULL);
}

void Server::errorMsg(int codeError, const std::string command, const std::string message, Client &client) const
{
    std::ostringstream ost;
    ost << ":" << this->_serverName << " " << codeError << " " << client.nickname << " " << command << " :" << message << "\r\n";
    send(client.fd, ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
}

void Server::invite()
{
    if (!clientList[nbrclient]->isRegistered) // error not registered
        return (errorMsg(451, cmd[0], "You have not registered", *clientList[nbrclient]), (void)0);
    if (cmd.size() < 3) // error not enough params
        return (errorMsg(461, cmd[0], "Not enough parameters", *clientList[nbrclient]), (void)0);
    int clientIndex = this->find_client(cmd[1]);
    Channel *channel = this->findChannelPtr(cmd[2]);
    if (clientList[nbrclient]->nickname == clientList[clientIndex]->nickname || clientIndex == -1) // error client introuvable ou host lui meme
        return (errorMsg(401, cmd[0], "No such nick", *clientList[nbrclient]), (void)0);
    if (!channel) // error channel n existe pas
        return (errorMsg(403, cmd[2], "No such channel", *clientList[nbrclient]), (void)0);
    if (!channel->isMember(clientList[nbrclient])) // error client qui invite n est pas dans le channel
        return (errorMsg(442, cmd[2], "You're not on that channel", *clientList[nbrclient]), (void)0);
    if (channel->getModes('i') && !channel->isOperator(clientList[nbrclient])) // error invite only et non operator
        return (errorMsg(482, cmd[2], "You're not channel operator", *clientList[nbrclient]), (void)0);
    if (channel->isMember(clientList[clientIndex])) // error guest deja sur la channel
        return (errorMsg(443, cmd[1], "is already on channel", *clientList[nbrclient]), (void)0);
    if (channel->isBannedClient(clientList[clientIndex])) // error guest is banned
        return (errorMsg(474, cmd[2], "Cannot join channel (+b)", *clientList[nbrclient]), (void)0);
    if (channel->channelIsFull()) // error channel is full
        return (errorMsg(471, cmd[1], "Cannot join channel (+l)", *clientList[nbrclient]), (void)0);
    channel->inviteClient(clientList[clientIndex]);
    std::ostringstream serverOst;
    std::ostringstream guestOst;
    serverOst << ":" << _serverName << " " << "341" << clientList[nbrclient]->nickname << " " << clientList[clientIndex]->nickname << " " << cmd[2] << "\r\n";
    guestOst << ":" << clientList[nbrclient]->nickname << " " << clientList[clientIndex]->nickname << " " << cmd[2] << "\r\n";
    send(clientList[nbrclient]->fd, serverOst.str().c_str(), serverOst.str().size(), MSG_DONTWAIT);
}
