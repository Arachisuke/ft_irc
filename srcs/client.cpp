#include "../header/client.hpp"

Client::Client()
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
    if (fd > 0)
    {
        close(this->fd);
        epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->fd, NULL); // CTLDEL
    }
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

void Client::ReadMsg(Server Server int nbrclient)
{
    
    char lecture[512];

    this->bytes = recv(this->fd, &lecture, sizeof(lecture), MSG_DONTWAIT);
    if (this->bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // pas de message a lire
            return(std::cout << "EAGAIIIIIIN" << std::endl, (void)0) ;
        return(Server.Close_client(nbrclient, "ERR_READ"));
    }
    else if (this->bytes == 0)
        return(Server.Close_client(nbrclient, "ERR_READ"));
    else if (this->bytes > 0)
    {
        this->buffer.append(lecture, this->bytes);
        size_t pos = buffer.find("\r\n");
        while(pos != std::string::npos)
        {    
            this->entry = this->buffer.substr(0, pos);
            this->buffer.erase(0, pos + 2);
            this->executeOrNot(Server, nbrclient);
            pos = this->buffer.find("\r\n");
        }
    }
}

int Client::executeOrNot(Server Server, int client_index) // inutile 
{
    if (this->entry.empty())
        return 0;
    if (this->find_cmd(Server, client_index)) // commande pas existante, ou prototype par respecter ou meme pas le bon contexte.
        std::cout << "ERROR CMD" << std::endl; //  le msg que la cmd renvoie ERR_... c'est dans la doc.

}

void Client::find_cmd(Server Server, int client_index)
{
    std::string cmd = this->entry.substr(0, this->entry.find(" "));
    this->entry = this->entry.substr(this->entry.find(" ") + 1); // ca fais bien le job? a tester.
    for (std::map<std::string, CommandFunc>::iterator it = commands.begin(); it != commands.end(); ++it) 
    {
        if (cmd == "PASS")
        {
            this->Pass(Server, client_index); // si le password est bon ca quitte et ca fais r, sinon ca quitte et ca ferme le client et ca push un msg d'erreur.
            return ;
        }
        else if (cmd == it->first)  
        {
            it->second();
            return ;
        }
    }
    std::cout << "Command not found" << std::endl;
}

void Client::load_cmd() // sans pass
{
    commands["CAP"] = &Client::Cap; // a ignorer.
    commands["NICK"] = &Client::Nick;
    commands["USER"] = &Client::User;
    commands["QUIT"] = &Client::Quit; // en plus du prototype ici il faut verifie que le client est bien connecte et dans le bon contexte.
    commands["JOIN"] = &Client::Join;
    commands["PART"] = &Client::Part;
    commands["PRIVMSG"] = &Client::PrivMsg;
    commands["NOTICE"] = &Client::Notice;
    commands["MODE"] = &Client::Mode;
    commands["TOPIC"] = &Client::Topic;
    commands["INVITE"] = &Client::Invite;
    commands["KICK"] = &Client::Kick;
    commands["PING"] = &Client::Ping;
}

// chaque debut de fonction
// verifier le nombre de params, et si le param correspond a la police par exemple ou a ce qui est attendu.
// bien connecte.
// bien dans le bon contexte.


void Client::PushMsg(std::string msg)
{ 
    msg.push_back('\r');
    msg.push_back('\n'); // a verifie si c'est vraiment la norme.
    send(this->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
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
    this->event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP; // surveille lecture et tout probleme.
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd, &this->event);
    this->load_cmd();
    return(0);
}

void Client::Send_Welcome() // rajouter le message 2 3 4.
{
    std::string welcome_msg =
    ":Hueco Mundo 001 " + this->nickname +
    " :Welcome to the Hueco Mundo Network, " + this->nickname + "!~" + this->username + "@localhost\r\n";
    send(this->fd, welcome_msg.c_str(), welcome_msg.size(), MSG_DONTWAIT);
    this->RPL_WELCOME = 1;
}

int Client::count_args() {
    std::istringstream iss(this->entry);
    std::string word;
    int count = 0;
    while (iss >> word) {
        ++count;
    }
    return count;
}