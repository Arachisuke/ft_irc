/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:06 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 15:28:12 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Channel *Server::findChannel(std::string &channelName)
{
    std::map<std::string, Channel *>::const_iterator it = channelList.find(channelName);
    if (it != channelList.end())
        return (it->second);
    return (NULL);
}

void Server::errorMsg(int codeEror, const std::string firstParam, const std::string secondParam, Client &client) const
{
    std::ostringstream ost;
    ost << ":" << "server" << " " << codeEror << " " << firstParam << " " << client.nickname << " " << secondParam << std::endl;
    send(client.fd, ost, sizeof(ost), MSG_DONTWAIT);
}

void Server::invite()
{
    if (!clientList[nbrclient]->isRegistered)
    {
        errorMsg(451, "INVITE", "You have not registered", *clientList[nbrclient]);
        return;
        // error not registered
    }
    if (cmd.size() < 2)
    {
        errorMsg(461, "INVITE", "Not enough parameters", *clientList[nbrclient]);
        return;
        // error not enough params
    }
    int clientIndex = find_client(cmd[0]);
    Channel *channel = findChannel(cmd[1]);
    if (clientList[nbrclient]->nickname == clientList[clientIndex]->nickname || clientIndex == -1)
    {
        errorMsg(401, "INVITE", "No such nick", *clientList[nbrclient]);
        return;
        // error client introuvable ou host lui meme
    }
    if (!channel)
    {
        errorMsg(403, cmd[1], "No such channel", *clientList[nbrclient]);
        return;
        // error channel n existe pas
    }
    if (!channel->isMember(clientList[nbrclient]))
    {
        errorMsg(442, cmd[1], "You're not on that channel", *clientList[nbrclient]);
        return;
        // error client qui invite n est pas dans le channel
    }
    if (channel->getOnInviteOnly() && !channel->isOperator(clientList[nbrclient]))
    {
        errorMsg(482, cmd[1], "You're not channel operator", *clientList[nbrclient]);
        return;
        // error invite only et non operator
    }
    if (channel->isMember(clientList[clientIndex]))
    {
        errorMsg(443, cmd[1], "is already on channel", *clientList[nbrclient]);
        return;
        // error guest deja sur la channel
    }
    channel->inviteClient(clientList[clientIndex]);
}
