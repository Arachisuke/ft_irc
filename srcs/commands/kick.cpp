/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:31 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/02 13:25:47 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

std::vector<int> Server::splitClients(std::string &clientsListToKick,std::string &kicker, Channel *channel, std::vector<std::string> &clientToKickOrdered)
{
    std::vector<std::string> splitClientToKick;
    std::stringstream ss(clientsListToKick);
    std::string buffer;

    while (std::getline(ss, buffer, ','))
        splitClientToKick.push_back(buffer);
    std::vector<int> splitClientTokickOrdered;
    std::vector<std::string>::iterator it = splitClientToKick.begin();
    for (; it < splitClientToKick.end(); it++)
    {
        int nbr = this->find_client((*it));
        if (nbr == -1 || kicker == _clientList[nbr]->getNickname())
        {
            errorMsg(401, _cmd[0], ERR_NOSUCHNICK, *_clientList[_nbrclient]);
            continue;
        }
        if (!channel->isMember(_clientList[nbr]))
        {
            errorMsg(441, _clientList[nbr]->getNickname(), ERR_USERNOTINCHANNEL, *_clientList[_nbrclient]);
            continue;
        }
        splitClientTokickOrdered.push_back(this->find_client((*it)));
        clientToKickOrdered.push_back((*it));
    }
    std::sort(splitClientTokickOrdered.begin(), splitClientTokickOrdered.end(), std::greater<int>());
    std::sort(clientToKickOrdered.begin(), clientToKickOrdered.end(), std::greater<std::string>());

    return (splitClientTokickOrdered);
}
void Server::broadcastMsg(Channel *channel, const char *msg, size_t size)
{
    for (std::set<Client *>::const_iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it)
        send((*it)->getFd(), msg, size, MSG_DONTWAIT);
}

void Server::kickAllClient(std::string &clientsListToKick, std::string kicker, Channel *channel, std::string reason)
{
    
    std::vector<std::string> ClientTokickOrdered;
    std::vector<int> clientsToKick = splitClients(clientsListToKick, kicker, channel, ClientTokickOrdered);
    std::vector<int>::iterator it = clientsToKick.begin();
    std::vector<std::string>::iterator ite = ClientTokickOrdered.begin();
    for (; it < clientsToKick.end() && ite < ClientTokickOrdered.end(); it++, ite++)
    {
        std::ostringstream ost;
        std::string addArobase = whatToDisplay(channel, this->_clientList[this->_nbrclient]);
        ost << ":" << addArobase << "!" << this->_clientList[this->_nbrclient]->getRealname() << "@localhost" << " KICK " << channel->getName() << " " << (*ite) << " :" << reason << "\r\n";
        broadcastMsg(channel, ost.str().c_str(), ost.str().size());
        channel->removeClient(_clientList[(*it)]);
        int i = whereIsMyChannel(channel->getName());
        _clientList[(*it)]->setMyChannel().erase(_clientList[(*it)]->setMyChannel().begin() + i);
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
