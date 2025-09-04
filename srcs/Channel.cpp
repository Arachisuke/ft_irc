/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:53:12 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/04 18:07:27 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _mode(""), _password(""), _maxUsers(-1)
{
}
Channel::~Channel()
{
}
bool Channel::isMember(Client *client) const
{
    return (_users.find(client) != _users.end());
}
bool Channel::isOperator(Client *client) const
{
    return (_operators.find(client) != _operators.end());
}

bool Channel::isInvited(Client *client) const
{
    return (_invited.find(client) != _invited.end());
}
void Channel::inviteClient(Client *client)
{
    if (isInvited(client))
        return;
    _invited.insert(client);
}
bool Channel::isBannedClient(Client *client) const
{
    return (_banned.find(client) != _banned.end());
}

void Channel::banClient(Client *client)
{
    _banned.insert(client);
}

void Channel::addOperator(Client *client)
{
    _operators.insert(client);
}
void Channel::removeClient(Client *clientToRemove)
{
    _operators.erase(clientToRemove);
    _users.erase(clientToRemove);
    _invited.erase(clientToRemove);
}
void Channel::removeOperator(Client *operatorToRemove)
{
    _operators.erase(operatorToRemove);
}

bool Channel::channelIsFull()
{
    if (_maxUsers < 0)
        return (0);
    return (_users.size() >= static_cast<unsigned long>(_maxUsers));
}
bool Channel::getModes(char modes) const
{
    for (std::set<char>::const_iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        if (*it == modes)
            return (1);
    }
    return (0);
}
const std::string &Channel::getTopic() const
{
    return (_topic);
}
const std::string &Channel::getTopicSetter() const
{
    return (_topicSetter);
}
void Channel::setModes(char modes)
{
    _modes.insert(modes);
}
void Channel::setMaxUsers(int maxUsers)
{
    _maxUsers = maxUsers;
}
void Channel::setName(const std::string &name)
{
    _name = name;
}
void Channel::setTopic(const std::string &newTopic)
{
    _topic = newTopic;
}
void Channel::setTopicSetter(const std::string &newTopicSetter)
{
    _topicSetter = newTopicSetter;
}
const std::set<Client *> &Channel::getUsers() const
{
    return (_users);
}

void Channel::setUsers(Client *client)
{
    _users.insert(client);
}
const std::string &Channel::getName() const
{
    return (_name);
}

int Channel::checkChannelNorm(const std::string &channelName) const
{
    if (channelName[0] != '#')
        return (0);
    if (channelName.length() > 50 || channelName.length() == 1)
        return (0);
    for (size_t i = 0; i < channelName.length(); i++)
    {
        if (channelName[i] == ' ' || channelName[i] == ',' || channelName[i] == '\r' || channelName[i] == '\n' || channelName[i] == '\0')
            return (0);
    }
    return (1);
}


