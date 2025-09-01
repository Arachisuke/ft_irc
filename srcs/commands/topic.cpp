/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:45:02 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/28 14:10:41 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Server::topic()
{
	if (!clientList[nbrclient]->isRegistered)
		return (errorMsg(451, cmd[0], "You have not registered", *clientList[nbrclient]), (void)0);
	if (cmd.size() < 3)
		return (errorMsg(461, cmd[0], "Not enough parameters", *clientList[nbrclient]), (void)0);
	Channel *channel = findChannelPtr(cmd[1]);
	if (!channel)
		return (errorMsg(403, cmd[2], "No such channel", *clientList[nbrclient]), (void)0);
	if (cmd.size() == 3)
	{
		if (channel->getTopic().empty())
			reply(331, cmd[1], ":No topic is set", *clientList[nbrclient]);
		else
		{
			reply(332, cmd[1], channel->getTopic(), *clientList[nbrclient]);
			reply(332, cmd[1], channel->getTopicSetter(), *clientList[nbrclient]);
		}
		return;
	}
	if (!channel->isMember(clientList[nbrclient]))
		return (errorMsg(442, cmd[2], "You're not on that channel", *clientList[nbrclient]), (void)0);
	if (channel->getModes('t') && !channel->isOperator(clientList[nbrclient]))
		return (errorMsg(482, cmd[2], "You're not channel operator", *clientList[nbrclient]), (void)0);
	std::string newTopic;
	if (cmd[3][0] == ':')
		newTopic = cmd[3].substr(1);
	else
		newTopic = cmd[3];
	channel->setTopic(newTopic);
	channel->setTopicSetter(clientList[nbrclient]->nickname);
	std::ostringstream topicAdvert;
	topicAdvert << ":" << clientList[nbrclient]->nickname << " TOPIC " << cmd[1] << " :" << newTopic << "\r\n";
	const std::set<Client *> &users = channel->getUsers();
	for (std::set<Client *>::const_iterator it = users.begin(); it != users.end(); it++)
		send((*it)->fd, topicAdvert.str().c_str(), topicAdvert.str().size(), MSG_DONTWAIT);
}