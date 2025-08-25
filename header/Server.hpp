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
  typedef void (Server::*CommandFunc)();
  Server();
  ~Server();
  int Init();
  void Finish();
  void closeClient(std::string ERROR_MSG);
  struct sockaddr_in hote;
  std::vector<Client *> clientList;  
  std::vector<Channel *> channeList;

  std::string password;
  std::string entry;
  std::string buffer;
  std::vector<std::string> cmd;
  void ReadMsg(std::string bufferClient);
  void PushMsg(std::string msg);


  int range_port(char *port);
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
  int findNick()


  void nick();
  int fd;
  int nbrclient;
  int epfd;
  int port;
  struct epoll_event events[5];
  std::map<std::string, CommandFunc> commandList;
  void load_cmd();
  void find_cmd();
  void executeOrNot();
  int create_server(char *password); // mettre le truc de reference.
  int find_client(int fd);
  int wait_client();

  int bytes;
};

#endif