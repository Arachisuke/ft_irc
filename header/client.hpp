/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:29:51 by ankammer          #+#    #+#             */
/*   Updated: 2025/08/11 15:36:13 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/epoll.h>
#include <vector>
#include <cstdlib>

class Client
{

public:
    Client();         // ajout constructeur par defaut
    Client(int hote); // pas implementee?
    ~Client();
    void ReadMsg();
    void PushMsg(std::string msg);
    int Init(int hote);
    int fd;
    int RPL_WELCOME;
    int RPL_INFO;
    int RPL;

    std::string nickname;   // 9 length
    std::string username;   // 9 length
    std::string realname;   // 128 length
    std::string hostname;   // 63 length
    std::string servername; // 63 length
    std::string mode;       // 3 length

    int Registration(int hote); // ajout declaration de la fonction car defini .cpp
private:
    std::string translationclient_to_server(std::string message);
    void Send_Welcome();              // ajout declaration de la fonction car defini .cpp
    void Client::Integrate(int hote); // ajout declaration de la fonction car defini .cpp
    int epfd;
    int hote;
    struct epoll_event events;
    struct sockaddr_in client;
    socklen_t size_of_client;
    std::vector<int> client_list;
    std::string password;
    std::string entry;
    int bytes;
};