/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:47 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/30 14:16:03 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

std::vector<std::string> Server::ft_split(const std::string &str, char delimiter)
{
  std::vector<std::string> result;
  std::string token;
  for (size_t i = 0; i < str.size(); ++i)
  {
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

void Server::privMsg()
{
  if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
    return (reply(451, this->_cmd[0], ERR_NOTREGISTERED, *this->_clientList[this->_nbrclient]), (void)0);
  if (this->_cmd.size() - 1 == 0)
    return (reply(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *this->_clientList[this->_nbrclient]), (void)0);
  std::vector<std::string> list;
  list = ft_split(this->_cmd[1], ',');
  for (size_t i = 0; i < list.size(); i++)
  {
    if (list[i][0] == '#')
    {
      if (findChannel(list[i]) == -1)
      {
        reply(403, this->_cmd[0], ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]);
        continue;
      }
      int n = findChannel(list[i]);
      if (!this->_channeList[n]->isMember(this->_clientList[this->_nbrclient]))
      {
        reply(404, this->_cmd[0], ERR_CANNOTSENDTOCHAN, *this->_clientList[this->_nbrclient]);
        continue;
      }
      std::string addArobase = whatToDisplay(_channeList[n], this->_clientList[this->_nbrclient]);
      std::string msg = ":" + addArobase + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " PRIVMSG " + this->_channeList[n]->getName() + " :";

      for (size_t i = 2; i < this->_cmd.size(); i++)
        msg += this->_cmd[i] + " ";
      msg += "\r\n";

      std::set<Client *> users = this->_channeList[n]->getUsers();
      for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
      {
        if ((*it)->getFd() != _clientList[_nbrclient]->getFd())
          send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
      }
    }
    else
    {
      std::string local = "@localhost";
      if (find_client(list[i]) == -1)
      {
        reply(401, this->_cmd[0], ERR_NOSUCHNICK, *this->_clientList[this->_nbrclient]);
        continue;
      }
      if (list[i] == this->_clientList[this->_nbrclient]->getNickname())
        continue ;
      int n = find_client(list[i]);
      if (this->_clientList[this->_nbrclient]->getNickname() == "WALL-E")
        local = "@thebot";
      std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + local + " PRIVMSG " + this->_clientList[n]->getNickname() + " :";
      for (size_t i = 2; i < this->_cmd.size(); i++)
        msg += this->_cmd[i] + " ";
      msg += "\r\n";
      send(this->_clientList[n]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
    }
  }
}