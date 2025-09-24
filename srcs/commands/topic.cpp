/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:45:02 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/24 15:01:17 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Server::topic()
{
	if (!_clientList[_nbrclient]->getisRegistered())
		return (errorMsg(451, _cmd[0], ERR_NOTREGISTERED, *_clientList[_nbrclient]), (void)0);
	if (_cmd.size() < 2)
		return (errorMsg(461, _cmd[0], ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]), (void)0);
	if (!this->checkChannelNorm(_cmd[1]))
		return (errorMsg(476, this->_cmd[0], ERR_BADCHANMASK, *_clientList[_nbrclient]), (void)0);
	Channel *channel = findChannelPtr(_cmd[1]);
	if (!channel)
		return (errorMsg(403, _cmd[1], ERR_NOSUCHCHANNEL, *_clientList[_nbrclient]), (void)0);
	if (_cmd.size() == 2)
	{
		if (channel->getTopic().empty())
			reply(331, _cmd[1], RPL_NOTOPIC, *_clientList[_nbrclient]);
		else
		{
			reply(332, _cmd[1], channel->getTopic(), *_clientList[_nbrclient]);
			reply(333, _cmd[1], channel->getTopicSetter(), *_clientList[_nbrclient]);
		}
		return;
	}
	if (!channel->isMember(_clientList[_nbrclient]))
		return (errorMsg(442, _cmd[1], ERR_NOTONCHANNEL, *_clientList[_nbrclient]), (void)0);
	if (channel->isModeActif('t') && !channel->isOperator(_clientList[_nbrclient]))
		return (errorMsg(482, _cmd[1], ERR_CHANOPRIVSNEEDED, *_clientList[_nbrclient]), (void)0);
	std::string newTopic;
	if (_cmd[2][0] == ':')
		newTopic = _cmd[2].substr(1);
	else
		newTopic = _cmd[2];
	channel->setTopic(newTopic);
	channel->setTopicSetter(_clientList[_nbrclient]->getNickname());
	std::ostringstream topicAdvert;
	std::string addArobase = whatToDisplay(channel, this->_clientList[this->_nbrclient]);
	topicAdvert << ":" << addArobase << "!" << _clientList[_nbrclient]->getUsername() << "@localhost TOPIC " << channel->getName() << " :" << newTopic << "\r\n";
	const std::set<Client *> &users = channel->getUsers();
	for (std::set<Client *>::const_iterator it = users.begin(); it != users.end(); it++)
		send((*it)->getFd(), topicAdvert.str().c_str(), topicAdvert.str().size(), MSG_DONTWAIT);
}