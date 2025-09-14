/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:34 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 12:44:35 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void   Server::mode()
{
  if (this->_cmd.size() - 1 == 0)
    return(reply(461, "MODE", "Not enough parameters", *this->_clientList[this->_nbrclient]), (void)0);
  if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
    return(reply(451, "MODE", "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);
  if (this->_cmd[1][0] == '#')
  {
    if (findChannel(this->_cmd[1]) == -1)
      return(reply(403, "MODE", ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]), (void)0);
    int n = findChannel(this->_cmd[1]);
      if (this->_cmd.size() - 1 == 1)
        {
          std::string msg = ":" + this->_serverName + " 324 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_channeList[n]->getName() +  this->_channeList[n]->getModes() + "\r\n";
          send(this->_clientList[this->_nbrclient].getfd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
          return (reply(324, _cmd[1], channel->getModes(), *_clientList[_nbrclient]), reply(329, _cmd[1], channel->getCreationDate(), *_clientList[_nbrclient]));
"\r\n";
          return(send(this->_clientList[this->_nbrclient].getfd(), msg2.c_str(), msg2.size(), MSG_DONTWAIT), (void)0);
        }
    
    for (size_t i = 0; i < this->_cmd[2].size(); i++)
      {
        if (this->_cmd[2][i] == '+' && this->_cmd[2][i + 1]) // ajout 
          {
               this->_channeList[n].setModes(this->_cmd[2][i + 1]); // j'ai recup la lettre
          }
            // i t k o l

        if (this->_cmd[2][i] == '-' && this->_cmd[2][i + 1]) // supression
            this->_channeList[n].suppModes(this->_cmd[2][i + 1]); // j'ai recup la lettre

            // i t k o l

      }
      
        
      
  }
  else
  {
    if (find_client(this->_cmd[1]) == -1)
      return(reply(401, "MODE", ERR_NOSUCHNICK, *this->_clientList[this->_nbrclient]), (void)0);
    int n = find_client(this->_cmd[1]);
    if (this->clientList[n]->getNickname() != this->_clientList[this->_nbrclient]->getNickname()))
      return(reply(502, "MODE", ERR_USERSDONTMATCH, *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_cmd.size() - 1 == 1)
      {
        std::string msg = ":" + this->_serverName + " 221 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_clientList[this->_nbrclient]->getMode() + "\r\n";
        return(send(this->_clientList[this->_nbrclient].getfd(), msg.c_str(), msg.size(), MSG_DONTWAIT), (void)0);

        if (this->_cmd[2][0] == '+') // ajout 
          // i t k o l

        if (this->_cmd[2][0] == '-') // supression
          // i t k o l
        
      }
    
  }
  
}
