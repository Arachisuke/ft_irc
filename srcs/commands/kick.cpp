/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:31 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/24 14:58:19 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

std::vector<std::string> splitClients(std::string &clientsListToKick)
{
    std::vector<std::string> clientToKick;
    std::stringstream ss(clientsListToKick);
    std::string buffer;

    while (std::getline(ss, buffer, ','))
        clientToKick.push_back(buffer);
    return (clientToKick);
}
void Server::broadcastMsg(Channel *channel, const char *msg, size_t size)
{
    for (std::set<Client *>::const_iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it)
        send((*it)->getFd(), msg, size, MSG_DONTWAIT);
}

void Server::kickAllClient(std::string &clientsListToKick, std::string kicker, Channel *channel, std::string reason)
{
    std::vector<std::string> clientToKick = splitClients(clientsListToKick);
    for (std::vector<std::string>::iterator it = clientToKick.begin(); it < clientToKick.end(); it++)
    {
        int clientIndex = this->find_client((*it));
        if (clientIndex == -1 || kicker == _clientList[clientIndex]->getNickname())
        {
            errorMsg(401, _cmd[0], ERR_NOSUCHNICK, *_clientList[_nbrclient]);
            continue;
        }
        if (!channel->isMember(_clientList[clientIndex]))
        {
            errorMsg(441, (*it), ERR_USERNOTINCHANNEL, *_clientList[_nbrclient]);
            continue;
        }
        std::ostringstream ost;
        std::string addArobase = whatToDisplay(channel, this->_clientList[this->_nbrclient]);
        ost << ":" << addArobase << "!" << this->_clientList[this->_nbrclient]->getRealname() << "@localhost" << " KICK " << channel->getName() << " " << (*it) << " :" << reason << "\r\n";
        broadcastMsg(channel, ost.str().c_str(), ost.str().size());
        channel->removeClient(_clientList[clientIndex]);
        int i = whereIsMyChannel(channel->getName());
        _clientList[clientIndex]->setMyChannel().erase(_clientList[clientIndex]->setMyChannel().begin() + i);
    }
}

void Server::kick()
{
    std::string reason;
    if (!_clientList[_nbrclient]->getisRegistered())
        return (errorMsg(451, _cmd[0], ERR_NOTREGISTERED, *_clientList[_nbrclient]), (void)0);
    if (_cmd.size() < 3)
        return (errorMsg(461, _cmd[0], ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]), (void)0);
    if (!this->checkChannelNorm(_cmd[1]))
        return (errorMsg(476, _cmd[0], ERR_BADCHANMASK, *_clientList[_nbrclient]));
    Channel *channel = findChannelPtr(_cmd[1]);
    if (!channel)
        return (errorMsg(403, _cmd[1], ERR_NOSUCHCHANNEL, *_clientList[_nbrclient]), (void)0);
    if (!channel->isMember(_clientList[_nbrclient]))
        return (errorMsg(442, _cmd[1], ERR_NOTONCHANNEL, *_clientList[_nbrclient]), (void)0);
    if (!channel->isOperator(_clientList[_nbrclient]))
        return (errorMsg(482, _cmd[1], ERR_CHANOPRIVSNEEDED, *_clientList[_nbrclient]), (void)0);
    if (_cmd.size() > 3)
        reason = _cmd[3];
    else
        reason = "kicked";
    kickAllClient(_cmd[2], _clientList[_nbrclient]->getNickname(), channel, reason);
}
