/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/11 19:58:23 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class Client
{

    public:
        Client( std::vector<Client*>& client_list, std::string password);
        ~Client();
        void Send_Welcome();
        std::vector<Client*>& client_to_client_list;
        void Big_3(std::vector<Client*>& client_list, int nbrclient, std::string ERROR_MSG);
        void Integrate();
        void Registration(int nbrclient);
        void ReadMsg(int nbrclient); 
        void PushMsg(std::string msg);
        int Init(int epfd, int hote);
        int fd;
        int RPL_WELCOME;

        int RPL_INFO;
        int Password_Status;
        int Nickname_Status;
        int Username_Status;
        std::string entry;




    std::string nickname; // 9 length
    std::string username; // 9 length
    std::string mode; // 3 length 
    private:
        std::string translationclient_to_server(std::string message);
        int epfd;
        int hote;
        struct epoll_event event;
        struct sockaddr_in client;
        socklen_t size_of_client;
        std::vector<int> client_list;
        std::string password;
        std::string buffer;
        int bytes;       
};

    #endif