/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:53:12 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/25 17:14:54 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _mode(""), _password(""), _onInviteOnly(0), _isMember(0), _isOperator(0), _maxUsers(-1)
{
}
Channel::~Channel()
{
}
bool Channel::getIsMember(const Client &client) const
{
    return (_isMember);
}
bool Channel::getIsOperator(const Client &client) const
{
    return (_isOperator);
}
bool Channel::getOnInviteOnly() const
{
    return (_onInviteOnly);
}
bool Channel::alreadyInvited(Client *client) const
{
    return (_invited.find(client) != _invited.end());
}
void Channel::inviteMember(Client *client)
{
    if (alreadyInvited(client))
        return;
    _invited.insert(client);
}
