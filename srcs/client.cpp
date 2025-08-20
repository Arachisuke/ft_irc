#include "../header/client.hpp"

Client::Client(std::vector<Client*>& client_list, std::string password) : client_to_client_list(client_list)
{
    this->password = password;
    this->RPL_WELCOME = 0;
    this->Password_Status = 0;
    this->Nickname_Status = 0;
    this->Username_Status = 0;
    this->isRegistered = 0;
    this->fd = -1;
}
Client::~Client()
{
    epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->fd, NULL); // CTLDEL
    if (fd > 0)
        close(this->fd);
    std::cout << "Client disconnected\r\n" << std::endl;
}

std::string Client::translationclient_to_server(std::string s)
{
    
    
     if (!s.empty() && s[s.size() - 1] == '\n')
          s.erase(s.size() - 1, 1);
    else
         
      if (!s.empty() && s[s.size() - 1] == '\r')
          s.erase(s.size() - 1, 1);

    return(s);
}

void Client::ReadMsg(int nbrclient)
{
    
    char lecture[512];

    this->bytes = recv(this->fd, &lecture, sizeof(lecture), MSG_DONTWAIT);
    if (this->bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // pas de message a lire
            return(std::cout << "EAGAIIIIIIN" << std::endl, (void)0) ;
        return(this->Big_3(this->client_to_client_list, nbrclient, "ERR_READ"));
    }
    else if (this->bytes == 0) // controleD cmd controleD .. != ... cmd controle cmd. si jenvoie cm controle D et D de cmd ...
    {
        std::cout << "BYTES == 0" << std::endl;
        return(this->Big_3(this->client_to_client_list, nbrclient, NULL));
    }
    else if (this->bytes > 0)
    {
        this->buffer.append(lecture, this->bytes); // je recup tout. 3 cas .. premier cas Pong / second cas pong/rn // troisiÃ¨me cas pong/rn/pong/rn
        size_t pos = buffer.find("\r\n");
        while(pos != std::string::npos)
        {    
            this->entry = this->buffer.substr(0, pos);
            this->buffer.erase(0, pos + 2);
            //this->parse_msg(nbrclient);
            pos = this->buffer.find("\r\n");
        }
        std::cout << "entry final :" << this->entry << std::endl;
        std::cout << "buffer :" << this->entry << std::endl;
    }
}

// int Client::(int client_index) // verifier s'il est co, et c'est quoi le msg envoyer.
// {
//     if (this->entry.empty()) // ca ne devrais pas arrive.
//         return 0;
//     if (cmd exist et bien ecrite)
    
//     if (cmd pas exist)
//     // verifier si elle est existante et bien ecrite avec les parametres // prendre son prototype dans ircdoc
//     if (this->isRegistered)
//         // lancer la cmd
//     else
//         // msg d'erreur associer a la cmd
//     return 0;
// }

void Client::PushMsg(std::string msg)
{
    
    if (this->RPL_WELCOME == 0 && this->Username_Status == 1)
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
    this->event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP; // Surveiller lecture (ajoute le client à epoll)
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->event);
    return(0);
}

void Client::Send_Welcome()
{
    std::string welcome_msg =
    ": Hueco Mundo 001 " + this->nickname +
    " : Welcome to the Hueco Mundo Network, " + this->nickname + "!~" + this->username + "@localhost\r\n";
    send(this->fd, welcome_msg.c_str(), welcome_msg.size(), MSG_DONTWAIT);
    this->RPL_WELCOME = 1;
}
void   Client:: Registration(int nbrclient)
{
    if (this->Username_Status)
        return ;
    if (this->Password_Status == 0)
    {
        this->ReadMsg(nbrclient);
        this->Password_Status = 1;
        if (this->entry != this->password)  // WRONG PASSWORD
            return (Big_3(this->client_to_client_list, nbrclient, "Wrong Password"));
        return ;
    }
    else if (this->Nickname_Status == 0)
    {
        if (!this->Password_Status)
            // msg d'erreur
        this->ReadMsg(nbrclient); // big 3
        this->nickname = this->entry;
        this->Nickname_Status = 1;
        return ;
    }
    else if (this->Username_Status == 0)
    {
        if (!this->Username_Status && !this->Nickname_Status)
        this->ReadMsg(nbrclient); // big 3
        this->username = this->entry;
        this->Username_Status = 1;
        this->isRegistered = 1;
    }
    this->event.events = EPOLLOUT; // Surveiller lecture (ajoute le client à epoll)
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->event);  
}
void Client::Big_3(std::vector<Client*>& client_list, int nbrclient, std::string ERROR_MSG)
{
    if (!ERROR_MSG.empty())
    {
        ERROR_MSG.push_back('\r');
        ERROR_MSG.push_back('\n'); // a verifie si c'est vraiment la norme.
        send(this->fd, ERROR_MSG.c_str(), ERROR_MSG.size(), MSG_DONTWAIT);
    }
    if (client_list[nbrclient])
        client_list.erase(client_list.begin() + nbrclient);
    std::cout << "Client disconnected\r\n" << std::endl;
    delete this;
    return ;
} 