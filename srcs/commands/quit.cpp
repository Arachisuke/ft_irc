/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/18 15:40:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Server::successfullQuit()
{
    std::vector<int> alreadysent;
    std::string msg = "";
    for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++)
    {
        const std::set<Client *> &users = this->_clientList[this->_nbrclient]->getMyChannel()[i]->getUsers();
        std::string addArobase = whatToDisplay(this->_clientList[this->_nbrclient]->getMyChannel()[i], this->_clientList[this->_nbrclient]);
        msg = ":" + addArobase + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " QUIT :Quit: ";
        if (this->_cmd.size() > 1)
            msg += this->_cmd[1];
        msg += "\r\n";
        for (std::set<Client *>::const_iterator it = users.begin(); it != users.end(); ++it)
        {
            if ((*it)->getFd() != this->_clientList[this->_nbrclient]->getFd() && std::find(alreadysent.begin(), alreadysent.end(), (*it)->getFd()) == alreadysent.end())
                send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
            alreadysent.push_back((*it)->getFd());
        }
    }
    msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " QUIT :Quit: ";
    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT); // pourquoi envoyer un msg au client qui quitte? a supprimmer?
}

void Server::quit()
{
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
        return (reply(451, _cmd[0], "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);
    this->successfullQuit();
    std::string msg2 = "ERROR :Closing Link: " + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost (Quit:";
    if (this->_cmd.size() > 1)
        msg2 += this->_cmd[1];
    msg2 += " )\r\n";

    send(this->_clientList[this->_nbrclient]->getFd(), msg2.c_str(), msg2.size(), MSG_DONTWAIT);
    for (int i = this->_clientList[this->_nbrclient]->getMyChannel().size() - 1; i >= 0; --i)
    {
        this->_clientList[this->_nbrclient]->getMyChannel()[i]->removeClient(this->_clientList[this->_nbrclient]);
        if (this->_clientList[this->_nbrclient]->getMyChannel()[i]->getUsers().empty())
        {
            int b = findChannel(this->_clientList[this->_nbrclient]->setMyChannel()[i]->getName());
            delete this->_clientList[this->_nbrclient]->setMyChannel()[i];
            this->_clientList[this->_nbrclient]->setMyChannel()[i] = NULL;
            this->_clientList[this->_nbrclient]->setMyChannel().erase(this->_clientList[this->_nbrclient]->setMyChannel().begin() + i);
            this->_channeList.erase(this->_channeList.begin() + b);
        }
    }

    throw std::runtime_error("");
}
