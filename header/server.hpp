/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/08/10 16:46:55 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "client.hpp"

class Server {
public:
  Server(std::vector<Client*>& client_list);
  ~Server();
  int Init(int epfd, int port);
  void close_all_client();
  struct sockaddr_in hote;
  std::vector<Client*>& server_to_client_list;
  std::string password;
  int fd;
  int epfd;
  struct epoll_event events[5];
  int bytes;
};

#endif