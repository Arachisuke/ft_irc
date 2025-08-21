/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/20 13:02:38 by ankammer         ###   ########.fr       */
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
#include <map>
#include <iostream>
#include <sstream>

#define MAX_EVENTS 10
#define ERR_NEEDMOREPARAMS ":Not Enough Parameters\r\n"
#define ERR_ALREADYREGISTRED ":You may not reregister\r\n"
#define ERR_PASSWDMISMATCH ":Password Incorrect\r\n"
#define ERR_ERRONEUSNICKNAME ":Erroneus Nickname\r\n"
#define ERR_NICKNAMEINUSE ":Nickname is already in use\r\n"
#define ERR_NONICKNAMEGIVEN ":No nickname given\r\n"
#define ERR_TOOMANYARGS ":Too many arguments\r\n"
#define ERR_UNKNOWNCOMMAND ":Unknown command\r\n"
#define ERR_NOSUCHNICK ":No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL ":No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN ":Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS ":Too many channels\r\n"
#define ERR_WASNOSUCHNICK ":There was no such nickname\r\n"





typedef int (*CommandFunc)(); // tester sans param apres on verra quand je vais le code.



class Client
{

    public:
        Client( std::vector<Client*>& client_list, std::string password);
        ~Client();
        void Send_Welcome();
        std::vector<Client*>& client_to_client_list;
        void Registration(int nbrclient);
        void ReadMsg(int nbrclient); 
        void PushMsg(std::string msg);
        int executeOrNot(int client_index);
        int Init(int epfd, int hote);
        int load_cmd();
        void find_cmd(Server Server, int client_index);
        int Pass(Server Server, int nbrclient);
        int fd;
        int count_args();

        int RPL_WELCOME;

        int RPL_INFO;
        int Password_Status;
        int Nickname_Status;
        int Username_Status;
        int isRegistered;
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
        std::map <std::string, CommandFunc> commands;
        socklen_t size_of_client;
        // std::vector<int> client_list;
        std::string password;
        std::string buffer;
        int bytes;       
};

    #endif