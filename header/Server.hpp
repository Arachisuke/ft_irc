/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/09/10 14:30:29 by ankammer         ###   ########.fr       */
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
#include "Channel.hpp"

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
  void ping();
  void kick();
  void invite();
  void topic();
  void mode();
  void notice();
  void privMsg();
  void user();
  void nick();
  void closeClient(std::string ERROR_MSG);

  void Finish();
  void Send_Welcome();
  void create_server(int port, char *password);
  void successfullJoin(int i);
  void successfullNick();
  void ReadMsg(std::string &bufferClient);
  void PushMsg(std::string msg);
  void errorMsg(int codeError, const std::string command, const std::string message, Client &client) const;
  void reply(int codeError, const std::string command, const std::string message, Client &client) const;
  void load_cmd();
  void find_cmd();
  void kickAllClient(std::string &clientToKick, std::string kicker, Channel *channel, std::string reason);
  void parseCmd(std::string &wordPrefixLess);
  void printParsedCmd();
  void broadcastMsg(Channel *channel, const char *msg, size_t size);
  void addModesChannel(Channel *channel,  std::vector<std::string> cmd);

  int findNick();
  int nickpolicy();
  int findChannel(std::string channel);
  int Init();
  int find_client(int fd);
  int isprint(char c);
  int find_client(std::string &nameClient);
  int wait_client();
  bool checkChannelNorm(const std::string &channelName) const;

  const std::string getPrefiksServer() const;

  Channel *findChannelPtr(std::string &channelName);

  std::vector<std::string> ft_split(const std::string &str, char delimiter);
  void successfullQuit(std::string msg);

private:
  int whereIsMyChannel(std::string channel);

  std::vector<Channel *> _channeList;
  std::vector<Client *> _clientList;
  std::vector<std::string> _cmd;
  std::map<std::string, CommandFunc> _commandList;

  struct sockaddr_in _hote;
  struct epoll_event _events[5];

  std::string _password;
  std::string _entry;
  std::string _buffer;
  std::string _serverName;

  ssize_t _bytes;
  int _fd;
  int _RPL_WELCOME;
  int _nbrclient;
  int _epfd;
  int _port;
};

#endif