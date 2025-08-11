/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankammer <ankammer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:23:54 by macos             #+#    #+#             */
/*   Updated: 2025/08/11 15:04:32 by ankammer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

class Server
{
public:
    Server();                               // si jamais utilise a deplacer en private pour respecter la forme canonique?
    Server(int port, std::string password); // ajout de la declation car defini .cpp
    ~Server();
    int Init(int epfd); // return void or int? changer en void a voir par walid
    struct sockaddr_in hote;
    int fd;
    int epfd;
    struct epoll_event events;
    int bytes;
    void clearsinzero(struct sockaddr_in *addr);

private:
    int _port; // ajout des attributs du server
    std::string _password;
};