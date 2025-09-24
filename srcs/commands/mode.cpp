/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:34 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/24 13:40:36 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void Server::addModesChannel(Channel *channel, std::vector<std::string> cmd)
{
	bool enoughParam = 0;
	bool addOrRemove = 1;
	int param = 3;
	std::string addArobase = whatToDisplay(channel, this->_clientList[this->_nbrclient]);
	if (cmd[2][0] == '-')
		addOrRemove = 0;
	if (cmd[2][0] == '+')
		addOrRemove = 1;
	for (size_t i = 1; i < cmd[2].length(); i++)
	{
		enoughParam = 1;
		std::ostringstream ost;
		ost << ":" << addArobase << "!" << this->_clientList[this->_nbrclient]->getRealname() << "@localhost " << _cmd[0] << " " << channel->getName() << " " << cmd[2][0] << cmd[2][i] << "\r\n";
		if (cmd[2][i] == 'i')
		{
			channel->setModes('i', addOrRemove);
			broadcastMsg(channel, ost.str().c_str(), ost.str().size());
		}
		else if (cmd[2][i] == 't')
		{
			channel->setModes('t', addOrRemove);
			broadcastMsg(channel, ost.str().c_str(), ost.str().size());
		}
		else if (cmd[2][i] == 'k')
		{
			if (!addOrRemove)
			{
				channel->setModes('k', addOrRemove);
				broadcastMsg(channel, ost.str().c_str(), ost.str().size());
				continue;
			}
			else if (static_cast<size_t>(param) >= cmd.size() || cmd[param].empty())
			{
				errorMsg(461, "+o", ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]);
				continue;
			}
			channel->setModes('k', addOrRemove);
			channel->setPassword(cmd[param++], addOrRemove);
			broadcastMsg(channel, ost.str().c_str(), ost.str().size());
		}
		else if (cmd[2][i] == 'o')
		{
			if (static_cast<size_t>(param) >= cmd.size() || cmd[param].empty())
			{
				errorMsg(461, "-o", ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]);
				continue;
			}
			int i = find_client(cmd[param++]);
			if (i == -1 || i == _nbrclient || !channel->isMember(_clientList[i]))
			{
				errorMsg(401, "+o", ERR_NOSUCHNICK, *_clientList[_nbrclient]);
				continue;
			}
			if (addOrRemove)
				channel->addOperator(_clientList[i]);
			else
				channel->removeOperator(_clientList[i]);
			broadcastMsg(channel, ost.str().c_str(), ost.str().size());
		}
		else if (cmd[2][i] == 'l')
		{
			if (!addOrRemove)
			{
				channel->setModes('l', addOrRemove);
				broadcastMsg(channel, ost.str().c_str(), ost.str().size());
				continue;
			}
			else if (static_cast<size_t>(param) < cmd.size() || !cmd[param].empty())
			{
				char *nptr;
				int maxUser = std::strtol(cmd[param++].c_str(), &nptr, 10);
				if (!*nptr && (maxUser > 1 && maxUser < 1000) && addOrRemove)
				{
					channel->setMaxUsers(maxUser);
					channel->setModes('l', addOrRemove);
				}
				else
				{
					errorMsg(400, "+l", ERR_UNKNOWNERROR, *_clientList[_nbrclient]);
					continue;
				}
				broadcastMsg(channel, ost.str().c_str(), ost.str().size());
			}
		}
		else
		{
			std::string unknownMode;
			unknownMode.push_back(cmd[2][i]);
			errorMsg(472, unknownMode, ERR_UNKNOWNMODE, *_clientList[_nbrclient]);
			continue;
		}
	}
	if (enoughParam)
		return;
	errorMsg(461, _cmd[2], ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]);
}

void Server::mode()
{
	if (!_clientList[_nbrclient]->getisRegistered())
		return (errorMsg(451, this->_cmd[0], ERR_NOTREGISTERED, *_clientList[_nbrclient]));
	if (this->_cmd.size() < 2)
		return (errorMsg(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *_clientList[_nbrclient]));
	if (_cmd[1] == this->_clientList[this->_nbrclient]->getNickname())
		return;
	if (!this->checkChannelNorm(_cmd[1]))
		return (errorMsg(476, _cmd[0], ERR_BADCHANMASK, *_clientList[_nbrclient]));
	Channel *channel = this->findChannelPtr(this->_cmd[1]);
	if (!channel)
		return (errorMsg(403, _cmd[2], ERR_NOSUCHCHANNEL, *_clientList[_nbrclient]));
	if (!channel->isMember(_clientList[_nbrclient]))
		return (errorMsg(442, _cmd[2], ERR_NOTONCHANNEL, *_clientList[_nbrclient]));
	if (_cmd.size() == 2 || (_cmd.size() > 2 && (_cmd[2][0] != '-' && _cmd[2][0] != '+')))
	{
		if (this->_clientList[this->_nbrclient]->getIrssi() == 1)
			return (reply(324, _cmd[1], channel->getModes(), *_clientList[_nbrclient]), reply(329, _cmd[1], channel->getCreationDateIrssi(), *_clientList[_nbrclient]));
		else
			return (reply(324, _cmd[1], channel->getModes(), *_clientList[_nbrclient]), reply(329, _cmd[1], channel->getCreationDate(), *_clientList[_nbrclient]));
	}
	if (!channel->isOperator(_clientList[_nbrclient]))
		return (errorMsg(482, _cmd[1], ERR_CHANOPRIVSNEEDED, *_clientList[_nbrclient]));
	addModesChannel(channel, _cmd);
}