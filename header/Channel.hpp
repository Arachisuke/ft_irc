/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:45:39 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/22 16:43:51 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Client.hpp"

#define MAX_EVENTS 10
#define ERR_NEEDMOREPARAMS "Not Enough Parameters"
#define ERR_ALREADYREGISTRED "You may not reregister"
#define ERR_PASSWDMISMATCH "Password Incorrect"
#define ERR_ERRONEUSNICKNAME "Erroneus Nickname"
#define ERR_NICKNAMEINUSE "Nickname is already in use"
#define ERR_NONICKNAMEGIVEN "No nickname given"
#define ERR_TOOMANYARGS "Too many arguments"
#define ERR_UNKNOWNCOMMAND "Unknown command"
#define ERR_NOSUCHNICK "No such nick/channel"
#define ERR_NOSUCHCHANNEL "No such channel"
#define ERR_CANNOTSENDTOCHAN "Cannot send to channel"
#define ERR_TOOMANYCHANNELS "Too many channels"
#define ERR_WASNOSUCHNICK "There was no such nickname"

class Client;

class Channel
{
public:
    Channel();
    ~Channel();

    int key;
    // Clients Status
    bool isMember(Client *client) const;
    bool isOperator(Client *client) const;
    bool isInvited(Client *client) const; // check set _invited si deja dans la liste
    bool isModeActif(char mode);
    bool channelIsFull();

    // Getters
    const std::string getModes() const;
    const std::string &getName() const;
    const std::string &getTopic() const;
    const std::string &getTopicSetter() const;
    const std::set<Client *> &getUsers() const;
    const std::string getCreationDate();
    const std::string getCreationDateIrssi() const;
    const std::string &getPassword() const;
    size_t getMaxUsers() const;

    // Setters
    void setModes(char modes, bool addOrRemove);
    void setMaxUsers(int maxUsers);
    void setName(const std::string &name);
    void setTopic(const std::string &newTopic);
    void setTopicSetter(const std::string &newTopicSetter);
    void setUsers(Client *client);
    void setPassword(std::string password, bool addOrRemove);
    void setCreationDate();
    void setCreationDate1();


    // Clients Management
    void inviteClient(Client *client);             // ajoute client a set _invited
    void addOperator(Client *newClient);           // ajoute client a set _operator
    void removeClient(Client *clientToRemove);     // remove from every set <>
    void removeOperator(Client *operatorToRemove); // remove from operator only
    void removeInvited(Client *invitedToRemove); // remove from invited only

private:
    std::string _name;
    std::string _topic;
    std::string _topicSetter;
    std::string _password; // mode +k_
    std::string _creationDate;
    std::string _creationDateirssi;
    time_t _presentTime;
    

    std::set<Client *> _users;     // all clients
    std::set<Client *> _operators; // client operator
    std::set<Client *> _invited;   // client invited
    std::set<char> _modes;         // modes actifs (itkl pas o)

    int _maxUsers; // mode +l
};

#endif