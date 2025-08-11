#include "../header/client.hpp"

Client::Client()
{
   this->RPL_WELCOME = 0;
}
Client::~Client()
{
    epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->fd, NULL); // CTLDEL
    close(this->fd);
    std::cout << "Client disconnected" << std::endl;
}

std::string Client::translationclient_to_server(std::string s)
{
     if (!s.empty() && s[s.size() - 1] == '\n')
          s.erase(s.size() - 1, 1);
      if (!s.empty() && s[s.size() - 1] == '\r')
          s.erase(s.size() - 1, 1);

    return(s);
}
void  Client::ReadMsg()
{
    char lecture[512];

  this->bytes = recv(this->fd, &lecture, sizeof(lecture), MSG_NOSIGNAL);
    if (this->bytes == -1)
    {
        std::cout << "ERR_READ" << std::endl;
        close(this->fd);
    }
    else if (this->bytes > 0)
    {
        std::string message(lecture, this->bytes);
        this->entry = this->translationclient_to_server(message);
    }
    // else == 0 / erreur
}
void Client::PushMsg(std::string msg)
{
    if (this->RPL_WELCOME == 0)
        return (this->Send_Welcome(), (void)0);
    msg.push_back('\r');
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(this->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
}

int Client::Init(int epfd, int hote)
{
    this->size_of_client = sizeof(this->client);
    this->epfd = epfd;
    this->hote = hote;
    this->fd = accept(hote, reinterpret_cast<sockaddr *>(&this->client), &size_of_client);
    if (fd == -1)
        return(-1);
    fcntl(this->fd, F_SETFL, O_NONBLOCK);

    return(0);
}

int Client::Registration() // est ce que le server doit lui ecrire un truc ou rien du tout.
{
    this->ReadMsg();
    if (entry != password) // Error on le laisse pas entrer
        return(1);
    this->ReadMsg();
    this->nickname = this->entry;
    this->ReadMsg();
    this->username = this->entry;
    this->Integrate();

    return(0);
}
void Client::Send_Welcome()
{
    std::string welcome_msg =
    ":Hueco Mundo 001 " + this->nickname +
    " :Welcome to the Hueco Mundo Network, " + this->nickname + "!~" + this->username + "@localhost\r\n";
    std::cout << welcome_msg;
    this->RPL_WELCOME = 1;
}

void Client::Integrate()
{
    this->event.events = EPOLLIN; // Surveiller lecture (ajoute le client à epoll)
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->event);
}