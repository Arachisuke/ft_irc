/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:31 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/08 16:56:54 by ankammer         ###   ########.fr       */
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

void Server::kickAllClient(std::string &clientsListToKick, std::string kicker, Channel *channel, std::string reason)
{
    std::vector<std::string> clientToKick = splitClients(clientsListToKick);
    for (std::vector<std::string>::iterator it = clientToKick.begin(); it < clientToKick.end(); it++)
    {
        int clientIndex = this->find_client((*it));
        if (clientIndex == -1 || kicker == _clientList[clientIndex]->getNickname())
        { // error client introuvable ou host lui meme
            errorMsg(401, _cmd[0], "No such nick", *_clientList[_nbrclient]);
            continue;
        }
        if (!channel->isMember(_clientList[clientIndex]))
        { // error client qui est kicked n est pas dans le channel
            errorMsg(441, (*it), "They aren't on that channel", *_clientList[_nbrclient]);
            continue;
        }
        if (channel->isOperator(_clientList[clientIndex]))
            continue;           // a voir
        std::ostringstream ost; // ajouter ici le message de remove
        ost << _clientList[_nbrclient]->getPrefiksClient() << " KICK " << channel->getName() << " " << (*it) << " :" << reason << "\r\n";
        for (std::set<Client *>::const_iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it)
            send((*it)->getFd(), ost.str().c_str(), ost.str().size(), MSG_DONTWAIT);
        channel->removeClient(_clientList[clientIndex]);
        int i = whereIsMyChannel(channel->getName());
        _clientList[clientIndex]->setMyChannel().erase(_clientList[clientIndex]->setMyChannel().begin() + i );
    }
}

void Server::kick()
{
    std::string reason;
    if (!_clientList[_nbrclient]->getisRegistered())
        return (errorMsg(451, _cmd[0], "You have not registered", *_clientList[_nbrclient]), (void)0);
    if (_cmd.size() < 3) // error not enough params
        return (errorMsg(461, _cmd[0], "Not enough parameters", *_clientList[_nbrclient]), (void)0);
    Channel *channel = findChannelPtr(_cmd[1]);
    if (!channel)
        return (errorMsg(403, _cmd[1], "No such channel", *_clientList[_nbrclient]), (void)0);
    if (!channel->checkChannelNorm(_cmd[1])) // error  invalid channel name
        return (errorMsg(476, _cmd[0], "Bad Channel Mask", *_clientList[_nbrclient]));
    if (!channel->isMember(_clientList[_nbrclient])) // error client qui kick n est pas dans le channel
        return (errorMsg(442, _cmd[1], "You're not on that channel", *_clientList[_nbrclient]), (void)0);
    if (!channel->isOperator(_clientList[_nbrclient])) // error non operator
        return (errorMsg(482, _cmd[1], "You're not channel operator", *_clientList[_nbrclient]), (void)0);
    if (!_cmd[3].empty())
        reason = _cmd[3];
    else
        reason = "kicked";
    kickAllClient(_cmd[2], _clientList[_nbrclient]->getNickname(), channel, reason);
}
