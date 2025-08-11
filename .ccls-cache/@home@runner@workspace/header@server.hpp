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

#include "client.hpp"

class Server {
public:
    Server();
    ~Server();
    Init(int epfd);
    struct sockaddr_in hote;
    int fd;
    int epfd;
    struct epoll_event events;
    int bytes;
};