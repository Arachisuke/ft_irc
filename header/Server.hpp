/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/09/01 15:02:49 by ankammer         ###   ########.fr       */
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
#include <map>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <csignal>
#include <vector>
#include <string>
#include <iostream>
#include "Client.hpp"

class Client;
class Channel;

class Server
{
public:
  typedef void (Server::*CommandFunc)();
  Server();
  ~Server();

  // Commands
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
  int findNick();
  void closeClient(std::string ERROR_MSG);
  int nickpolicy();
  int findChannel(std::string channel);
  int imInOrNot(std::string channel);

  int Init();
  void create_server(int port, char *password);
  void Finish();
  int find_client(int fd);
  int find_client(std::string &nameClient);
  int wait_client();

  Channel *findChannelPtr(std::string &channelName);
  void errorMsg(int codeError, const std::string command, const std::string message, Client &client) const;

void  successfullJoin(int i);
void successfullNick();
  void ReadMsg(std::string &bufferClient);
  void PushMsg(std::string msg);

  int isprint(char c);

  void reply(int codeError, const std::string command, const std::string message, Client &client) const;
  void load_cmd();
  void find_cmd();
  void executeOrNot();
  std::vector<std::string> ft_split(const std::string& str, char delimiter);


  // private:
  std::vector<Client *> clientList;
  std::vector<Channel *> channeList;
  int whereIsChannel(std::string channel);

  std::vector<std::string> cmd;
  std::map<std::string, CommandFunc> commandList;
  struct sockaddr_in hote;
  struct epoll_event events[5];
  std::string password;
  std::string entry;
  std::string buffer;
  int fd;
  std::string _serverName;
  int bytes;
  int nbrclient;
  int epfd;
  int port;
};

#endif