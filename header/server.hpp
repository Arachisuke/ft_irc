/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wzeraig <wzeraig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/08/21 13:16:07 by wzeraig          ###   ########.fr       */
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
#include "client.hpp"

class Server
{
public:
  Server();
  ~Server();
  int Init(int epfd, int port);
  void Finish();
  void closeClient(int nbrclient, std::string ERROR_MSG);
  struct sockaddr_in hote;
  std::vector<Client *> clientList;
  std::string password;
  int fd;
  int epfd;
  struct epoll_event events[5];
  int bytes;

  int join();
  int quit();
  int part();
  int pass();
  int cap();
  int ping();
  int kick();
  int invite();
  int topic();
  int mode();
  int notice();
  int privMsg();
  int user();
  int nick();
};

#endif