/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:31 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/06 13:14:23 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

std::vector<int> Server::splitClients(std::string &clientsListToKick,std::string &kicker, Channel *channel, std::vector<std::string> &clientNameToKick)
{
    std::stringstream ss(clientsListToKick);
    std::string buffer;
    std::set<std::pair<int, std::string>, std::greater<std::pair<int, std::string> > > pairNickIndex;
    std::vector<std::string> tempNames;
    
    while (std::getline(ss, buffer, ','))
        tempNames.push_back(buffer);
    for (std::vector<std::string>::iterator it = tempNames.begin(); it < tempNames.end(); it++)
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
        pairNickIndex.insert(std::make_pair(nbr, *it));
    }
    std::vector<int> clientIndexToKick;
    for (std::set<std::pair<int, std::string>, std::greater<std::pair<int, std::string> > >::iterator it = pairNickIndex.begin(); it != pairNickIndex.end(); ++it)
    {
        clientIndexToKick.push_back(it->first);
        clientNameToKick.push_back(it->second);
    }

    return (clientIndexToKick);
}
void Server::broadcastMsg(Channel *channel, const char *msg, size_t size)
{
    for (std::set<Client *>::const_iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it)
        send((*it)->getFd(), msg, size, MSG_DONTWAIT);
}

void Server::kickAllClient(std::string &clientsListToKick, std::string kicker, Channel *channel, std::string reason)
{
    
    std::vector<std::string> clientNameToKick;
    std::vector<int> clientIndexToKick = splitClients(clientsListToKick, kicker, channel, clientNameToKick);
    std::vector<int>::iterator it = clientIndexToKick.begin();
    std::vector<std::string>::iterator ite = clientNameToKick.begin(); 
    // 
    for (; it != clientIndexToKick.end(); it++, ite++)
    {
        std::ostringstream ost;
        std::cout << "NAME : " << *ite << " INDEX : " <<  *it << std::endl;
        std::string addArobase = whatToDisplay(channel, this->_clientList[this->_nbrclient]);
        ost << ":" << addArobase << "!" << this->_clientList[this->_nbrclient]->getRealname() << "@localhost" << " KICK " << channel->getName() << " " << (*ite) << " :" << reason << "\r\n";
        broadcastMsg(channel, ost.str().c_str(), ost.str().size());
        channel->removeClient(_clientList[(*it)]);
        
        std::vector<Channel *> &myChannel = _clientList[(*it)]->setMyChannel();
        for (size_t i = 0; i < myChannel.size(); i++)
        {
            if (myChannel[i]->getName() == channel->getName())
            {
                myChannel.erase(myChannel.begin() + i);
                break;
            }
        }
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
