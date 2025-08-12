#include "../header/client.hpp"

Client::Client(std::vector<Client*>& client_list, std::string password) : client_to_client_list(client_list)
{
    this->password = password;
    this->RPL_WELCOME = 0;
    this->Registration_Status = 0;
}
Client::~Client()
{
    epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->fd, NULL); // CTLDEL
    if (fd > 0)
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
int Client::ReadMsg()
{
    char lecture[512];

    if (this->Registration_Status == 0)
    {
        if (this->Registration())
            return(send(this->fd, "Wrong Password\r\n", 16, MSG_DONTWAIT), 1);
    }
    this->bytes = recv(this->fd, &lecture, sizeof(lecture), 0);
    if (this->bytes == -1)
    {
        std::cout << "ERR_READ" << std::endl;
        close(this->fd);
        return(1);
    }
    else if (this->bytes > 0)
    {
        std::string message(lecture, this->bytes);
        this->entry = this->translationclient_to_server(message);
    }
    // else == 0 / erreur
    return(0);
}
void Client::PushMsg(std::string msg)
{
    if (this->RPL_WELCOME == 0)

        this->Send_Welcome();
    else
    {
        msg.push_back('\r');
        msg.push_back('\n'); // a verifie si c'est vraiment la norme.
        send(this->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
    }
    this->event.events = EPOLLIN;
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->event);
}

int Client::Init(int epfd, int hote)
{
    this->size_of_client = sizeof(this->client);
    this->epfd = epfd;
    this->hote = hote;

    this->fd = accept(this->hote, reinterpret_cast<sockaddr *>(&this->client), &size_of_client);
    if (fd == -1)
        return(-1);
    fcntl(this->fd, F_SETFL, O_NONBLOCK);
    this->Integrate();
    return(0);
}

int Client::Registration()
{
    this->Registration_Status = 1;
    this->ReadMsg();
    if (entry != password)
        return(1);
    this->ReadMsg();
    this->nickname = this->entry;
    this->ReadMsg();
    this->username = this->entry;
    this->event.events = EPOLLOUT;
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->event);
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
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->event);
}