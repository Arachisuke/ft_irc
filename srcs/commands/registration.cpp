
#include "Client.hpp"
#include "Server.hpp"

void   Server::pass(int nbrclient)
{
    if (this->cmd.size() - 1 > 1)
           return(std::cout << "ERRTOOMANYARGS" << std::endl, (void)0);
    if (this->Username_Status) // deja connecte.
        return ;
    if (this->Password_Status == 0)
    {
        this->ReadMsg(nbrclient);
        this->Password_Status = 1;
        if (this->entry != this->password)
            return(this->closeClient(nbrclient, "Password incorrect"));
        return ;
    }
    return (std::cout << "462 :ERRALREADYREGISTRED" << std::endl, (void)0);

}
void   Server::nick()
{
    if (this->cmd.size() - 1 == 0)
            // ERRNONICKNAMEGIVEN
    else if (this->count_args() > 1)
            // ERRTOOMANYARGS
    if // nickname deja pris
            // ERRNICKNAMEINUSE
    if // police pas respecter
            // ERRERRONEUSNICKNAME
    else if (this->Nickname_Status == 0)
    {
        if (!this->Password_Status)
            std::cout << "ERR_Registration" << std::endl; // changer ?
        
        this->ReadMsg(nbrclient);
        this->nickname = this->entry; // verifier la police ?
        this->Nickname_Status = 1;
        return ;
    }
}
void   Server::user()
{
    if (this->cmd.size() - 1 > 4)
            // ERRTOOMANYARGS
        if (this->cmd.size() - 1 < 4)
            // ERRNEEDMOREPARAMS
    
    else if (this->Username_Status == 0)
    {
        if (!this->Username_Status && !this->Nickname_Status)
            std::cout << "ERR_Registration" << std::endl; // changer ?
        this->ReadMsg(nbrclient);
        this->username = this->entry; // verifier la police ? verifier sil est vide pour l'erreur verifie la lenght
        this->Username_Status = 1;
        this->isRegistered = 1;
    }
    else
        // ERRALREADYREGISTRED
    this->events.events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->events.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->events); 
}



// password meme s'il est faux j'arrete pas la registration