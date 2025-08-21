
void   Client::Pass(Server Server, int nbrclient)
{
    if (this->count_args() > 1)
           return(std::cout << "ERRTOOMANYARGS" << std::endl, (void)0);
    if (this->Username_Status) // deja connecte.
        return ;
    if (this->Password_Status == 0)
    {
        this->ReadMsg(nbrclient);
        this->Password_Status = 1;
        if (this->entry != this->password)
            return(Server.Close_client(nbrclient, "Password incorrect"));
        return ;
    }
    return (std::cout << "462 :ERRALREADYREGISTRED" << std::endl, (void)0);

}
void   Client::Nick(int nbrclient)
{
    if (this->count_args() == 0)
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
void   Client::User(int nbrclient)
{
    if (this->count_args() > 4)
            // ERRTOOMANYARGS
        if (this->count_args() < 4)
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
    this->event.events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
    this->event.data.fd = this->fd;
    epoll_ctl(this->epfd, EPOLL_CTL_MOD, this->fd, &this->event); 
}