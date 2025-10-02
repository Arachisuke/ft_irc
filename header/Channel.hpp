/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:45:39 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/01 13:55:58 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

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
    bool isInvited(Client *client) const; 
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