/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:13:43 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/06 14:05:45 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Channel.hpp"
#include "../../header/Client.hpp"
#include "../../header/Server.hpp"

int Server::findChannel(std::string channel)
{
	for (size_t i = 0; i < this->_channeList.size(); i++)
	{
		if (this->_channeList[i]->getName() == channel)
			return (i);
	}
	return (-1);
}

int Server::whereIsMyChannel(std::string channel)
{
	for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++)
	{
		if (this->_clientList[this->_nbrclient]->getMyChannel()[i]->getName() == channel)
			return (i);
	}
	return (-1);
}

void Server::successfullJoin(int i)
{
	std::set<Client *> users = this->_channeList[i]->getUsers();
	for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
	{
		std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost JOIN " + this->_channeList[i]->getName() + "\r\n";
		send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
	}
	if (this->_channeList[i]->getTopic() != "")
	{
		std::string topic = ":" + this->_serverName + " 332 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_channeList[i]->getName() + " :" + this->_channeList[i]->getTopic() + "\r\n";
		send(this->_clientList[this->_nbrclient]->getFd(), topic.c_str(),
			 topic.size(), MSG_DONTWAIT);
	}
	std::string msg = ":" + this->_serverName + " 353 " + this->_clientList[this->_nbrclient]->getNickname() + " = " + this->_channeList[i]->getName() + " :";
	for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
		msg += whatToDisplay(this->_channeList[i], *(it)) + " ";
	msg += "\r\n";
	send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(),
		 MSG_DONTWAIT);
	std::string msg2 = ":" + this->_serverName + " 366 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_channeList[i]->getName() + " :End of /NAMES list" + "\r\n";
	send(this->_clientList[this->_nbrclient]->getFd(), msg2.c_str(),
		 msg2.size(), MSG_DONTWAIT);
}

void Server::join()
{
	if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
		return (reply(451, this->_cmd[0], ERR_NOTREGISTERED, *this->_clientList[this->_nbrclient]), (void)0);
	if (this->_cmd.size() - 1 == 0)
		return (reply(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *this->_clientList[this->_nbrclient]), (void)0);
	if (this->_cmd[1] == "#0" || this->_cmd[1] == "0")
	{
		std::vector<Channel *> myChannel = this->_clientList[this->_nbrclient]->getMyChannel();
		for (int i = myChannel.size() - 1; i >= 0; i--)
		{
			if (!myChannel[i]->isMember(this->_clientList[this->_nbrclient]))
				continue;
			std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost PART " + myChannel[i]->getName();
			if (this->_cmd.size() > 2 && this->_cmd[2] != "")
				msg += " :" + this->_cmd[2];
			msg += "\r\n";

			std::set<Client *> users = myChannel[i]->getUsers();
			for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
				send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);

			myChannel[i]->removeClient(this->_clientList[this->_nbrclient]);
			this->_clientList[this->_nbrclient]->removeMyChannel(myChannel[i]);

			if (myChannel[i]->getUsers().empty())
			{
				this->_channeList.erase(this->_channeList.begin() + findChannel(myChannel[i]->getName()));
				delete myChannel[i];
				myChannel[i] = NULL;
			}
		}
		return;
	}

	else
	{
		std::vector<std::string> list = ft_split(this->_cmd[1], ',');
		size_t keynbr = 0;
		for (size_t j = 0; j < list.size(); j++)
		{
			if (!checkChannelNorm(list[j]))
			{
				reply(476, this->_cmd[0], ERR_BADCHANMASK, *this->_clientList[this->_nbrclient]);
				continue;
			}

			if (findChannel(list[j]) != -1)
			{
				int i = findChannel(list[j]);
				if (this->_channeList[i]->isModeActif('i') && !_channeList[i]->isInvited(_clientList[_nbrclient]))
				{
					reply(473, list[j], ERR_INVITEONLYCHAN, *this->_clientList[this->_nbrclient]);
					continue;
				}

				if (this->_channeList[i]->isModeActif('k'))
				{
					if (_cmd.size() < 3)
					{
						reply(475, list[j], ERR_BADCHANNELKEY, *this->_clientList[this->_nbrclient]);
						continue;
					}
					std::vector<std::string> key = ft_split(this->_cmd[2], ',');

					if (key[keynbr] != this->_channeList[i]->getPassword())
					{
						keynbr++;
						reply(475, list[j], ERR_BADCHANNELKEY, *this->_clientList[this->_nbrclient]);
						continue;
					}
					if (keynbr < key.size() - 1)
						keynbr++;
				}
				if (this->_channeList[i]->isModeActif('l') && this->_channeList[i]->getUsers().size() >= this->_channeList[i]->getMaxUsers())
				{
					reply(471, list[j], ERR_CHANNELISFULL, *this->_clientList[this->_nbrclient]);
					continue;
				}

				if (this->_channeList[i]->isMember(this->_clientList[this->_nbrclient]))
				{
					reply(443, this->_channeList[i]->getName(), ERR_USERONCHANNEL, *this->_clientList[this->_nbrclient]);
					continue;
				}
				if (this->_channeList[i]->isInvited(this->_clientList[this->_nbrclient]))
					this->_channeList[i]->removeInvited(this->_clientList[this->_nbrclient]);
				this->_channeList[i]->setUsers(this->_clientList[this->_nbrclient]);
				this->_clientList[this->_nbrclient]->setMyChannel().push_back(this->_channeList[i]);
				this->successfullJoin(i);
			}
			else
			{
				Channel *newChannel = new Channel();
				newChannel->setCreationDate();
				newChannel->setName(list[j]);
				this->_channeList.push_back(newChannel);
				newChannel->setUsers(this->_clientList[this->_nbrclient]);
				this->_clientList[this->_nbrclient]->setMyChannel().push_back(newChannel);
				newChannel->addOperator(this->_clientList[this->_nbrclient]);
				newChannel->setTopicSetter(this->_clientList[this->_nbrclient]->getNickname());
				newChannel->setTopic("");
				this->successfullJoin(this->_channeList.size() - 1);
			}
		}
	}
}