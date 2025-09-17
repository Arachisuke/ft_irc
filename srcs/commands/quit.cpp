/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/17 12:42:30 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Server::successfullQuit(std::string msg)
{
    std::vector<int> alreadysent;
    for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++)
    {
        const std::set<Client *> &users = this->_clientList[this->_nbrclient]->getMyChannel()[i]->getUsers();
        for (std::set<Client *>::const_iterator it = users.begin(); it != users.end(); ++it)
        {
            if ((*it)->getFd() != this->_clientList[this->_nbrclient]->getFd() && std::find(alreadysent.begin(), alreadysent.end(), (*it)->getFd()) == alreadysent.end())
                send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
            alreadysent.push_back((*it)->getFd());
        }
    }

    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);// pourquoi envoyer un msg au client qui quitte? a supprimmer? 
}




void   Server::quit()
{
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
        return (reply(451, "QUIT", "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);
    std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " QUIT :Quit: ";
    if (this->_cmd.size() > 1)
        msg += this->_cmd[1];
    msg += "\r\n";
    this->successfullQuit(msg);
    std::string msg2 = "ERROR :Closing Link: " + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost (Quit: ";
    if (this->_cmd.size() > 1)
        msg2 += ' ' + this->_cmd[1];
    msg2 += " )\r\n";

    send(this->_clientList[this->_nbrclient]->getFd(), msg2.c_str(), msg2.size(), MSG_DONTWAIT);

    for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++) // avant suppression client, vider la liste des channels du client et si channel vide erase et delete voir avec Walid
    {
        this->_clientList[this->_nbrclient]->getMyChannel()[i]->removeClient(this->_clientList[this->_nbrclient]);
        if (this->_clientList[this->_nbrclient]->getMyChannel()[i]->getUsers().empty())
        delete this->_clientList[this->_nbrclient]->getMyChannel()[i];
    }

    



    
    throw std::runtime_error("");
}
// for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getMyChannel().size(); i++)
// {
//     Channel *channel = this->_clientList[this->_nbrclient]->getMyChannel()[i];
//     channel->removeClient(this->_clientList[this->_nbrclient]);
//     if (channel->getUsers().empty())
//     {
//         for (size_t j = 0; j < _channeList.size(); j++)
//         {
//             if (channel == _channeList[j])
//             {
//                 this->_channeList.erase(this->_channeList.begin() + j);
//                 break;
//             }
//         }
//         delete channel;
//     }
// }
// this->closeClient("");
// return;
