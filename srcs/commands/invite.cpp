/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:06 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/01 15:21:29 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <sstream>

Channel *Server::findChannelPtr(std::string &channelName)
{
    for (size_t i = 0; i < this->_channeList.size(); i++)
    {
        if (this->_channeList[i]->getName() == channelName)
            return (this->_channeList[i]);
    }
    return (NULL);
}

void Server::errorMsg(int codeError, const std::string command, const std::string message, Client &client) const
{
    std::ostringstream ost;
    ost << ":" << this->_serverName << " " << codeError << " " << client.getNickname() << " " << command << " :" << message << "\r\n";
    send(client.getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
}

void Server::invite()
{
    if (!_clientList[_nbrclient]->getisRegistered()) // error not registered
        return (errorMsg(451, _cmd[0], ERR_NOTREGISTERED, *_clientList[_nbrclient]), (void)0);
    if (_cmd.size() < 3) // error not enough params
        return (errorMsg(461, _cmd[0], ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]), (void)0);
    int clientIndex = this->find_client(_cmd[1]);
    if (clientIndex == -1 || _clientList[_nbrclient]->getNickname() == _clientList[clientIndex]->getNickname()) // error client introuvable ou host lui meme
        return (errorMsg(401, _cmd[0], ERR_NOSUCHNICK, *_clientList[_nbrclient]), (void)0);
    if (!this->checkChannelNorm(_cmd[2])) // error invalid channel name
        return (errorMsg(476, _cmd[0], ERR_BADCHANMASK, *_clientList[_nbrclient]));
    Channel *channel = this->findChannelPtr(_cmd[2]);
    if (!channel) // error channel n existe pas
        return (errorMsg(403, _cmd[2], ERR_NOSUCHCHANNEL, *_clientList[_nbrclient]), (void)0);
    if (!channel->isMember(_clientList[_nbrclient])) // error client qui invite n est pas dans le channel
        return (errorMsg(442, _cmd[2],ERR_NOTONCHANNEL, *_clientList[_nbrclient]), (void)0);
    if (channel->isModeActif('i') && !channel->isOperator(_clientList[_nbrclient])) // error invite only et non operator
        return (errorMsg(482, _cmd[2], ERR_CHANOPRIVSNEEDED, *_clientList[_nbrclient]), (void)0);
    if (channel->isMember(_clientList[clientIndex])) // error guest deja sur la channel
        return (errorMsg(443, _cmd[1], ERR_USERONCHANNEL, *_clientList[_nbrclient]), (void)0);
    if (channel->channelIsFull()) // error channel is full
        return (errorMsg(471, _cmd[1], ERR_CHANNELISFULL, *_clientList[_nbrclient]), (void)0);
    channel->inviteClient(_clientList[clientIndex]);
    std::ostringstream serverOst;
    std::ostringstream guestOst;
    std::string addArobase = whatToDisplay(channel, this->_clientList[this->_nbrclient]);
    serverOst << ":" << _serverName << " 341 " << _clientList[_nbrclient]->getNickname() << " " << _clientList[clientIndex]->getNickname() << " " << _cmd[2] << "\r\n";
    guestOst << ":" << addArobase << "!" << _clientList[_nbrclient]->getUsername() << "@localhost INVITE " << _clientList[clientIndex]->getNickname() << " " << _cmd[2] << "\r\n";
    send(_clientList[_nbrclient]->getFd(), serverOst.str().c_str(), serverOst.str().size(), MSG_DONTWAIT);
    send(_clientList[clientIndex]->getFd(), guestOst.str().c_str(), guestOst.str().size(), MSG_DONTWAIT);
}
