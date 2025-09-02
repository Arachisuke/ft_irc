#include "../header/Server.hpp"
#include "../header/Client.hpp"

Client::Client()
{
    this->RPL_WELCOME = 0;
    this->Password_Status = 0;
    this->Nickname_Status = 0;
    this->Username_Status = 0;
    this->isRegistered = 0;
    this->fd = -1;
}
Client::~Client()
{
    
    std::cout << "Client disconnected\r\n"
              << std::endl;
}

int Client::Init(int epfd, int hote)
{
    this->size_of_client = sizeof(this->client);
    this->hote = hote;

    this->fd = accept(this->hote, reinterpret_cast<sockaddr *>(&this->client), &size_of_client);
    if (fd == -1)
        return (1);
    //fcntl(this->fd, F_SETFL, O_NONBLOCK);
    this->event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP; // surveille lecture et tout probleme.
    this->event.data.fd = this->fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, this->fd, &this->event);
    return (0);
}

