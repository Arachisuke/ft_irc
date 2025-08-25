/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:53:12 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/25 18:35:06 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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
void Channel::addClient(Client *newClient)
{
    _users.insert(newClient);
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
bool Channel::getOnInviteOnly() const
{
    return (_onInviteOnly);
}