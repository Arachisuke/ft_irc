/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:35:30 by macos             #+#    #+#             */
/*   Updated: 2025/08/21 14:25:46 by wzeraig          ###   ########.fr       */
/*
 */
/* ************************************************************************** */

#include "../header/Server.hpp"
#include "../header/Client.hpp"

bool stop = false;

void handler(int) { stop = true; }

int main(int argc, char **argv)
{
    if (argc != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    signal(SIGINT, &handler);
    Server server;

    server.range_port(argv[1]);
    if (!server.port)
        return(1);
    try
    {
        server.create_server(argv[2]);
        while (!stop)
            server.wait_client();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return 0;
}