/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/30 16:26:38 by wzeraig          ###   ########.fr       */
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
}

void Server::quit()
{
    this->successfullQuit();
    throw std::runtime_error("");
}
