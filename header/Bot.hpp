/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:51:45 by ankammer          #+#    #+#             */
/*   Updated: 2025/09/23 13:01:36 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_CLASS_HPP
#define BOT_CLASS_HPP

#include <iostream>
#include <sys/socket.h>
#include <string>

class Bot
{
public:
    Bot(std::string nickName, int port, std::string password, int fd);
    void connectBot();

private:
    std::string _nickName;
    std::string _password;
    int _port;
    int _fd;
};
#endif