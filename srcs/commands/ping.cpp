/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:44 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/26 12:44:45 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void   Server::ping() // un ou plusieurs arg a renvoye
{
    if (this->cmd.size() - 1 == 0)
        return(reply(461, "PING", "Not Enough Parameters", *this->clientList[this->nbrclient]), (void)0);
    this->PushMsg("HuecoMundo: PONG :" + this->cmd[1]);
}