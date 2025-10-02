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

#include "Client.hpp"
#include "Server.hpp"

bool stop = false;

void handler(int)
{
	stop = true;
}

int range_port(char *port)
{
	const char *s = port;
	char *end;
	long val;

	errno = 0;
	val = strtol(s, &end, 10);
	if (errno == ERANGE || val > 65535 || val <= 0)
		return (std::cerr << "Port number out of range." << std::endl, -1);
	if (*end != '\0')
		return (std::cerr << "Invalid port number." << std::endl, -1);
	return (static_cast<int>(val));
}

int main(int argc, char **argv)
{
	if (argc != 3)
		return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
	signal(SIGINT, &handler);
	signal(SIGQUIT, &handler);
	signal(SIGPIPE, SIG_IGN);
	Server server;
	int port = range_port(argv[1]);
	if (port < 0)
		return (1);
	try
	{
		server.create_server(port, argv[2]);
		while (!stop)
			server.wait_client();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}