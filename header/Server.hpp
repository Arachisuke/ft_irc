/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/10/02 13:33:42 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <csignal>
#include <map>
#include <sstream>
#include <set>
#include <functional>
#include "Client.hpp"
#include "Channel.hpp"

#define MAX_EVENTS 10
#define ERR_NEEDMOREPARAMS "Not Enough Parameters"
#define ERR_ALREADYREGISTRED "You may not reregister"
#define ERR_PASSWDMISMATCH "Password Incorrect"
#define ERR_ERRONEUSNICKNAME "Erroneus Nickname"
#define ERR_NICKNAMEINUSE "Nickname is already in use"
#define ERR_NONICKNAMEGIVEN "No nickname given"
#define ERR_TOOMANYARGS "Too many arguments"
#define ERR_UNKNOWNCOMMAND "Unknown command"
#define ERR_NOSUCHNICK "No such nick"
#define ERR_NOSUCHCHANNEL "No such channel"
#define ERR_CANNOTSENDTOCHAN "Cannot send to channel"
#define ERR_TOOMANYCHANNELS "Too many channels"
#define ERR_WASNOSUCHNICK "There was no such nickname"
#define ERR_USERSDONTMATCH "Cant change mode for other users"
#define ERR_NOTREGISTERED "You have not registered"
#define ERR_BADCHANMASK "Bad Channel Mask"
#define RPL_NOTOPIC "No topic is set"
#define ERR_USERONCHANNEL "is already on channel"
#define ERR_NOTONCHANNEL "You're not on that channel"
#define ERR_CHANOPRIVSNEEDED "You're not channel operator"
#define ERR_CHANNELISFULL "Cannot join channel (+l)"
#define ERR_UNKNOWNMODE "is unknown mode char to me)"
#define ERR_INVITEONLYCHAN "Cannot join channel (+i)"
#define ERR_BADCHANNELKEY "Cannot join channel (+k)"
#define ERR_USERNOTINCHANNEL "They aren't on that channel"
#define ERR_UNKNOWNERROR "Invalid user limit"

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
  void addModesChannel(Channel *channel, std::vector<std::string> cmd);
  std::vector<int> splitClients(std::string &clientsListToKick, std::string &kicker, Channel *channel, std::vector<std::string> &clientToKickOrdered);

  int findNick();
  int nickpolicy();
  int findChannel(std::string channel);
  int Init();
  int find_client(int fd);
  int isprint(char c);
  int find_client(std::string &nameClient);
  int wait_client();
  bool checkChannelNorm(const std::string &channelName) const;

  std::string whatToDisplay(Channel *channel, Client *client);
  const std::string getPrefiksServer() const;

  Channel *findChannelPtr(std::string &channelName);

  std::vector<std::string> ft_split(const std::string &str, char delimiter);
  void successfullQuit();

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