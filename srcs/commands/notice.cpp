/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:37 by ankammer          #+#    #+#             */
/*   Updated: 2025/10/01 15:21:17 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"

void Server::notice()
{
  std::vector<std::string> list;
  if (this->_cmd.size() - 1 == 0)
    return;
  if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
    return;
  list = ft_split(this->_cmd[1], ',');
  for (size_t i = 0; i < list.size(); i++)
  {
    if (list[i][0] == '#')
    {
      if (findChannel(list[i]) == -1)
      continue;
      int n = findChannel(list[i]);
      if (!this->_channeList[n]->isMember(this->_clientList[this->_nbrclient]))
      continue;
      
      std::string addArobase = whatToDisplay(_channeList[n], this->_clientList[this->_nbrclient]);
      std::string msg = ":" + addArobase + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " NOTICE " + this->_channeList[n]->getName() + " :";

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
      if (find_client(list[i]) == -1)
        continue;
      int n = find_client(list[i]);
      std::string addArobase = whatToDisplay(_channeList[n], this->_clientList[this->_nbrclient]);
      std::string msg = ":" + addArobase + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " NOTICE " + this->_clientList[n]->getNickname() + " :";
      for (size_t i = 2; i < this->_cmd.size(); i++)
        msg += this->_cmd[i] + " ";
      msg += "\r\n";
      send(this->_clientList[n]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
    }
  }
}
