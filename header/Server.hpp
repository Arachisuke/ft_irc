/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/08/21 16:45:38 by wzeraig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <csignal>
#include "Client.hpp"

class Client;

class Server
{
public:
  typedef void (Server::*CommandFunc)(); // tester sans param apres on verra quand je vais le code.

  Server();
  ~Server();
  int Init(int epfd, int port);
  void Finish();
  void closeClient(int nbrclient, std::string ERROR_MSG);
  struct sockaddr_in hote;
  std::vector<Client *> clientList;
  std::string password;
  std::string entry;
  std::string buffer;
  std::vector<std::string> cmd;
  void ReadMsg(int nbrclient, std::string bufferClient);
  void PushMsg(int nbrclient, std::string msg);

  void join();
  void quit();
  void part();
  void pass();
  void cap();
  void ping();
  void kick();
  void invite();
  void topic();
  void mode();
  void notice();
  void privMsg();
  void user();

  void nick();
  int fd;
  int epfd;
  struct epoll_event events[5];
  std::map<std::string, CommandFunc> commandList;
  void load_cmd();
  void find_cmd();
  void executeOrNot();
  int create_server(int port, char *password); // mettre le truc de reference.
  int find_client(std::vector<Client *> client_list, int fd);
  int wait_client();

  int bytes;
};

#endif