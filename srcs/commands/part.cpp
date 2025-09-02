/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:41 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 12:44:42 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

void   Server::part()
{
    if (this->cmd.size() - 1 == 0)
        return(reply(461, "PART", "Not Enough Parameters", *this->clientList[this->nbrclient]), (void)0);
    if (this->clientList[this->nbrclient]->isRegistered == 0)
        return(reply(451, "PART", "You have not registered", *this->clientList[this->nbrclient]), (void)0);
    std::vector<std::string> list;
    list = ft_split(this->cmd[1], ',');
    for (size_t j = 0; j < list.size(); j++)
          {
            if (list[j][0] != '#')
                return(reply(403, "PART", ERR_NOSUCHCHANNEL, *this->clientList[this->nbrclient]), (void)0);
            if (findChannel(list[j]) == -1)
                return(reply(403, "PART", ERR_NOSUCHCHANNEL, *this->clientList[this->nbrclient]), (void)0);
            int i = findChannel(list[j]);
            if (!this->channeList[i]->isMember(this->clientList[this->nbrclient]))
                return(reply(442, "PART", "You're not on that channel", *this->clientList[this->nbrclient]), (void)0);
            std::string msg = ":" + this->clientList[this->nbrclient]->nickname + "!" + this->clientList[this->nbrclient]->username + "@localhost" + " PART " + this->channeList[i]->getName();
            if (this->cmd[2] != "")
                msg += " :" + this->cmd[2];
            msg += "\r\n";
            std::set<Client *> users = this->channeList[i]->getUsers();
            for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
                send((*it)->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
              this->channeList[i]->removeClient(this->clientList[this->nbrclient]); // 2 
            if (this->channeList[i]->getUsers().empty()) 
                delete this->channeList[i]; // 1
              int b = whereIsChannel(this->channeList[i]->getName());
              this->clientList[this->nbrclient]->listofchannel.erase(this->clientList[this->nbrclient]->listofchannel.begin() + b); // 3
               
            }     
}

 // 1 supprimer le channel si il est vide.
// 2 supprimer le client du channel.
// 3 supprimer le channel de la liste du client.
// msg -> nickame + channelist[i].getname + reason du coup cmd[2]