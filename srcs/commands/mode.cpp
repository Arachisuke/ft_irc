/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:34 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/11 11:03:12 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

// // MODE ANDY

void Server::addModesChannel(Channel *channel, std::vector<std::string> cmd)
{
    bool addOrRemove = 1;
    int param = 3;
    if (cmd[2][0] == '-')
        addOrRemove = 0;
    if (cmd[2][0] == '+')
        addOrRemove = 1;
    for (size_t i = 1; i < cmd[2].length(); i++)
    {
        if (cmd[2][i] == 'i')
        {
            channel->setModes('i', addOrRemove);
        }
        else if (cmd[2][i] == 't')
        {
            channel->setModes('t', addOrRemove);
        }
        else if (cmd[2][i] == 'k')
        {
            if (!addOrRemove)
                channel->setModes('k', addOrRemove);
            else if (static_cast<size_t>(param) >= cmd.size() || cmd[param].empty())
            {
                errorMsg(461, _cmd[2], "Not enough parameters", *_clientList[_nbrclient]);
                continue;
            }
            channel->setModes('k', addOrRemove);
            channel->setPassword(cmd[param++], addOrRemove);
        }
        else if (cmd[2][i] == 'o')
        {
            if (static_cast<size_t>(param) >= cmd.size() || cmd[param].empty())
            {
                errorMsg(461, _cmd[2], "Not enough parameters", *_clientList[_nbrclient]);
                continue;
            }
            int i = find_client(cmd[param++]);
            if (i == -1 || i == _nbrclient)
            {
                errorMsg(401, _cmd[2], "No such nick", *_clientList[_nbrclient]);
                continue;
            }
            if (addOrRemove)
                channel->addOperator(_clientList[i]);
            else
                channel->removeOperator(_clientList[i]);
        }
        else if (cmd[2][i] == 'l')
        {
            if (!addOrRemove)
                channel->setModes('l', addOrRemove);
            else if (static_cast<size_t>(param) < cmd.size() || !cmd[param].empty())
            {
                char *nptr;
                int maxUser = std::strtol(cmd[param++].c_str(), &nptr, 10);
                if (!*nptr && (maxUser > 1 && maxUser < 1000) && addOrRemove)
                {
                    channel->setMaxUsers(maxUser);
                    channel->setModes('l', addOrRemove);
                }
                else
                {
                    errorMsg(400, "MODE", "Invalid user limit", *_clientList[_nbrclient]);
                    continue;
                }
            }
        }
        else
        {
            errorMsg(461, _cmd[2], "Not enough parameters", *_clientList[_nbrclient]);
            continue;
        }
    }
}

void Server::mode()
{
    if (!_clientList[_nbrclient]->getisRegistered())
        return (errorMsg(451, this->_cmd[0], "You have not registered", *_clientList[_nbrclient]));
    if (this->_cmd.size() < 2)
        return (errorMsg(461, this->_cmd[0], "Not enough parameters", *_clientList[_nbrclient]));
    if (!this->checkChannelNorm(_cmd[1]))
        return (errorMsg(476, "TOPIC", "Bad Channel Mask", *_clientList[_nbrclient]));
    Channel *channel = this->findChannelPtr(this->_cmd[1]);
    if (!channel)
        return (errorMsg(403, _cmd[2], "No such channel", *_clientList[_nbrclient]));
    if (!channel->isMember(_clientList[_nbrclient]))
        return (errorMsg(442, _cmd[2], "You're not on that channel", *_clientList[_nbrclient]));
    if (_cmd.size() == 2 || (_cmd.size() > 2 && (_cmd[2][0] != '-' && _cmd[2][0] != '+')))
        return (reply(324, _cmd[1], channel->getModes(), *_clientList[_nbrclient]), reply(329, _cmd[1], channel->getCreationDate(), *_clientList[_nbrclient]));
    if (!channel->isOperator(_clientList[_nbrclient]))
        return (errorMsg(482, _cmd[1], "You're not channel operator", *_clientList[_nbrclient]));
    addModesChannel(channel, _cmd);
}

// // MODE WALID

// void   Server::mode()
// {
//   if (this->_cmd.size() - 1 == 0)
//     return(reply(461, "MODE", "Not enough parameters", *this->_clientList[this->_nbrclient]), (void)0);
//   if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
//     return(reply(451, "MODE", "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);
//   if (this->_cmd[1][0] == '#')
//   {
//     if (findChannel(this->_cmd[1]) == -1)
//       return(reply(403, "MODE", ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]), (void)0);
//     int n = findChannel(this->_cmd[1]);
//       if (this->_cmd.size() - 1 == 1)
//         {
//           std::string msg = ":" + this->_serverName + " 324 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_clientList[n]->getName() +  this->_clientList[n]->getModes() + "\r\n";
//           send(this->_clientList[this->_nbrclient].getfd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
//           std::string msg2 = ":" + this->_serverName + " 329 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_clientList[n]->getName() + // rajouter la date de creation + "\r\n";
//           return(send(this->_clientList[this->_nbrclient].getfd(), msg2.c_str(), msg2.size(), MSG_DONTWAIT), (void)0);
//         }
//     if (!this->_channeList[n]->isOperator(this->_clientList[this->_nbrclient]))
//       return(reply(482, "MODE", "You're not channel operator", *this->_clientList[this->_nbrclient]), (void)0);

//       if (this->_cmd[2][0] == '+') // ajout
//           // i t k o l

//       if (this->_cmd[2][0] == '-') // supression
//           // i t k o l

//   }
//   else
//   {
//     if (find_client(this->_cmd[1]) == -1)
//       return(reply(401, "MODE", ERR_NOSUCHNICK, *this->_clientList[this->_nbrclient]), (void)0);
//     int n = find_client(this->_cmd[1]);
//     if (this->clientList[n]->getNickname() != this->_clientList[this->_nbrclient]->getNickname()))
//       return(reply(502, "MODE", ERR_USERSDONTMATCH, *this->_clientList[this->_nbrclient]), (void)0);
//     if (this->_cmd.size() - 1 == 1)
//       {
//         std::string msg = ":" + this->_serverName + " 221 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_clientList[this->_nbrclient]->getMode() + "\r\n";
//         return(send(this->_clientList[this->_nbrclient].getfd(), msg.c_str(), msg.size(), MSG_DONTWAIT), (void)0);

//         if (this->_cmd[2][0] == '+') // ajout
//           // i t k o l

//         if (this->_cmd[2][0] == '-') // supression
//           // i t k o l

//       }

//   }

// }
