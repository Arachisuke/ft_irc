/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:44 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/30 16:12:36 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void   Server::ping()
{
    if (this->_cmd.size() - 1 == 0)
            return(reply(461, this->_cmd[0], ERR_NEEDMOREPARAMS, *this->_clientList[this->_nbrclient]), (void)0);
    this->PushMsg("HuecoMundo: PONG " + this->_cmd[1]);
}