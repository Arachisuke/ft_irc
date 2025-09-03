/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:47 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/03 17:02:30 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

std::vector<std::string> Server::ft_split(const std::string& str, char delimiter) 
{
    std::vector<std::string> result;
    std::string token;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == delimiter) 
        {
            result.push_back(token);
            token.clear();
        } 
        else 
            token += str[i];
    }
    result.push_back(token);
    return result;
}

void   Server::privMsg()
{
  std::vector<std::string> list;
    if (this->_cmd.size() - 1 == 0)
      return(reply(461, "PRIVMSG", "Not Enough Parameters", *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
  return(reply(451, "PRIVMSG", "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);
    list = ft_split(this->_cmd[1], ',');
    for (size_t i = 0; i < list.size(); i++)
    {
        if (list[i][0] == '#')
        {
            if (findChannel(list[i]) == -1)
              return(reply(403, "PRIVMSG", ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]), (void)0);
            int n = findChannel(list[i]);
           std::string msg = ":" + this-> _clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " PRIVMSG " + this->_channeList[n]->getName() + " : ";
            
            for (size_t i = 2; i < this->_cmd.size(); i++)
                msg += this->_cmd[i] + " ";
            msg += "\r\n";
            
            std::set<Client *> users = this->_channeList[n]->getUsers();
            for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
                send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
        }
        else // client
        {
            if (find_client(list[i]) == -1)
              return(reply(401, "PRIVMSG", ERR_NOSUCHNICK, *this->_clientList[this->_nbrclient]), (void)0);
            int n = find_client(list[i]);
            std::string msg = ":" + this-> _clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " PRIVMSG " + this->_clientList[n]->getNickname() + " : ";
            for (size_t i = 2; i < this->_cmd.size(); i++)
                    msg += this->_cmd[i] + " ";
                msg += "\r\n";
            send(this->_clientList[n]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
        }
    }
}