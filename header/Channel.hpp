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
    bool getIsMember(const Client &client) const;
    bool getIsOperator(const Client &client) const;
    bool getOnInviteOnly() const;
    bool alreadyInvited(Client *client) const; // check set _invited si deja dans la liste
    void inviteMember(Client *client);               // ajoute client a set _invited

private:
    std::string _name;
    std::string _topic;
    std::string _mode;
    std::string _password; // mode +k
    int _maxUsers;         // mode +l

    bool _onInviteOnly;
    bool _isMember;
    bool _isOperator;
    std::set<Client*> _users;     // user classique
    std::set<Client*> _operators; // user operator
    std::set<Client*> _invited;   // user invited
    std::set<Client*> _banned;    // user banned
};

#endif