/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:41 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/04 14:28:41 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"
#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

void   Server::part()
{
    if (this->_cmd.size() - 1 == 0)
        return(reply(461, "PART", "Not Enough Parameters", *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
        return(reply(451, "PART", "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);
    std::vector<std::string> list;
    list = ft_split(this->_cmd[1], ',');
    for (size_t j = 0; j < list.size(); j++)
          {
            if (list[j][0] != '#')
                return(reply(403, "PART", ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]), (void)0);
            if (findChannel(list[j]) == -1)
                return(reply(403, "PART", ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]), (void)0);
            int i = findChannel(list[j]);
            if (!this->_channeList[i]->isMember(this->_clientList[this->_nbrclient]))
                return(reply(442, "PART", "You're not on that channel", *this->_clientList[this->_nbrclient]), (void)0);
            std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " PART " + this->_channeList[i]->getName();
            if (this->_cmd.size() > 2 && this->_cmd[2] != "")
                msg += " : " + this->_cmd[2];
            msg += "\r\n";
            std::set<Client *> users = this->_channeList[i]->getUsers();
            for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
                send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
              int b = whereIsMyChannel(this->_channeList[i]->getName());
              this->_clientList[this->_nbrclient]->setMyChannel().erase(this->_clientList[this->_nbrclient]->setMyChannel().begin() + b); // 3
              this->_channeList[i]->removeClient(this->_clientList[this->_nbrclient]); // 2 
              if (this->_channeList[i]->getUsers().empty()) 
                  delete this->_channeList[i]; // 1
            }     
}

 // 1 supprimer le channel si il est vide.
// 2 supprimer le client du channel.
// 3 supprimer le channel de la liste du client.
// msg -> nickame + _channelist[i].getname + reason du coup _cmd[2]