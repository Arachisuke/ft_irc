/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:45:39 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 15:43:08 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Client.hpp"

class Client;

class Channel
{
public:
    Channel();
    ~Channel();

    // Clients Status
    bool isMember(Client *client) const;
    bool isOperator(Client *client) const;
    bool isInvited(Client *client) const;      // check set _invited si deja dans la liste
    bool isBannedClient(Client *client) const; // check set _banned si deja dans la liste

    // Channel status
    bool getOnInviteOnly() const;

    //Channel Management
    void setModes(char modes);

    // Clients Management
    void inviteClient(Client *client);   // ajoute client a set _invited
    void banClient(Client *client);      // ajoute client a set _banned
    void addClient(Client *newClient);   // ajoute client a set _users
    void addOperator(Client *newClient); // ajoute client a set _operator
    void removeClient(Client *clientToRemove); // remove from every set <>
    void removeOperator(Client *operatorToRemove); //remove from operator only

private:
    std::string _name;
    std::string _topic;
    std::string _mode;
    std::string _password; // mode +k
    int _maxUsers;         // mode +l

    bool _onInviteOnly;

    std::set<Client *> _users;     // client classique
    std::set<Client *> _operators; // client operator
    std::set<Client *> _invited;   // client invited
    std::set<Client *> _banned;    // client banned
    std::set<char> _modes;         // modes actifs
};

#endif