/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:53:12 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/23 18:17:40 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _password(""), _creationDate(""), _maxUsers(-1)
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

void Channel::removeInvited(Client *invitedToRemove)
{
    _invited.erase(invitedToRemove);
}
bool Channel::channelIsFull()
{
    if (_maxUsers < 0)
        return (0);
    return (_users.size() >= static_cast<unsigned long>(_maxUsers));
}
const std::string Channel::getModes() const
{
    std::string modes = "";
    for (std::set<char>::const_iterator it = _modes.begin(); it != _modes.end(); it++)
        modes += *it;
    return (modes);
}

const std::string &Channel::getTopic() const
{
    return (_topic);
}
const std::string &Channel::getTopicSetter() const
{
    return (_topicSetter);
}
void Channel::setModes(char modes, bool addOrRemove)
{
    if (addOrRemove == 1)
        _modes.insert(modes);
    else
    {
        std::set<char>::iterator it = _modes.find(modes);
        if (it != _modes.end())
            _modes.erase(it);
    }
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
    time_t presentTime = time(NULL);                 // nombre de secondes ecoulee depuis creation unix 1 janvier 1970
    struct tm *localeTime = localtime(&presentTime); // convertit les secondes dans une structures (D/Y/M/...)
    char date[30];
    strftime(date, sizeof(date), "%b %d %Y %H:%M:%S", localeTime); // formate selon un pattern dans un buffer
    _topicSetter = newTopicSetter + " " + date;
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

bool Channel::isModeActif(char mode)
{
    for (std::set<char>::const_iterator it = _modes.begin(); it != _modes.end(); it++)
        if (mode == *it)
            return (1);
    return (0);
}

void Channel::setCreationDate()
{
    _presentTime = time(NULL);
    std::ostringstream oss;
    oss << _presentTime;
    _creationDateirssi = oss.str();
}

const std::string Channel::getCreationDate()
{
    struct tm *localeTime = localtime(&_presentTime); // convertit les secondes dans une structures (D/Y/M/...)
    char date[30];
    strftime(date, sizeof(date), "%a %b %d %H:%M:%S %Y", localeTime);
    _creationDate = date;
    return (_creationDate);
}
const std::string Channel::getCreationDateIrssi() const
{
    return (_creationDateirssi);
}

void Channel::setPassword(std::string password, bool addOrRemove)
{
    if (addOrRemove)
        this->_password = password;
    else
        this->_password = "";
}
const std::string &Channel::getPassword() const
{
    return (_password);
}

size_t Channel::getMaxUsers() const
{
    return (_maxUsers);
}
