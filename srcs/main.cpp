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

int range_port(char *port)
{
    const char *s = port;
    char *end;
    errno = 0;
    long val = strtol(s, &end, 10);
    if (errno == ERANGE || val > 65535 || val <= 0)
        std::cerr << "Port number out of range." << std::endl;
    if (*end != '\0')
        std::cerr << "Invalid port number." << std::endl;
    int n = static_cast<int>(val);
    return (n);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    signal(SIGINT, &handler);
    Server server;

    int n = range_port(argv[1]); // a mettre dans le try and catch .

    try
    {
        server.create_server(n, argv[2]);
        while (!stop)
            server.wait_client();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}