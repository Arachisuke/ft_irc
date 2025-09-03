/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:44:44 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/02 18:08:17 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void   Server::ping() // un ou plusieurs arg a renvoye
{
    if (this->_cmd.size() - 1 == 0)
            return(std::cout << "ERR_NEEDMOREPARAMS" << std::endl, (void)0);
    this->PushMsg("HuecoMundo: PONG" + this->_cmd[1]);
}