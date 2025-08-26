
#include "Client.hpp"
#include "Server.hpp"

void   Server::pass()
{
    if (this->cmd.size() - 1 == 0)
            return(std::cout << ERR_NEEDMOREPARAMS << std::endl, (void)0);
    if (this->clientList[this->nbrclient]->isRegistered == 1) 
        return (std::cout << "462:" << ERR_ALREADYREGISTRED << std::endl, (void)0);
    this->clientList[this->nbrclient]->Password_Status = 1;
    if (this->cmd[1] != this->password)
        this->clientList[this->nbrclient]->Password_Status = -1;
}

int   Server::findNick()
{
    for (size_t i = 0; i < this->clientList.size(); i++)
    {
        if (this->clientList[i]->nickname == this->entry)
            return (1);
    }
    return (0);
}

int   Server::nickpolicy()
{
    if (this->cmd[1][0] == ':' || this->cmd[1][0] == '#' || this->cmd[1][0] == '&' || this->cmd[1][0] == '!' || this->cmd[1][0] == '@')
        return (1);
    for (size_t i = 0; i < this->cmd[1].size(); i++)
        {
            if (!isprint(this->cmd[1][i]))
                return (1);
            if (this->cmd[1][i] == ':')
                return (1);
        }
    if (this->cmd[1].size() > 9)
        return (1);
    return (0);
}

int   Server::isprint(char c)
{
    if (c < 33 || c > 126)
        return (1);
    return (0);
}
    
void   Server::nick() // toomanyarg ??
{
    if (this->cmd.size() - 1 == 0)
            std::cout << ERR_NONICKNAMEGIVEN << std::endl;
    else if (this->clientList[this->nbrclient]->Nickname_Status == -1)
    {
        if (!this->clientList[this->nbrclient]->Password_Status)
            std::cout << "ERR_NEEDPASSWORDBEFORE" << std::endl; // A changer ? 
        
        if (nickpolicy())
            std::cout << ERR_ERRONEUSNICKNAME << std::endl;
        if (findNick())
            return (std::cout << ERR_NICKNAMEINUSE << std::endl, (void)0);
        this->clientList[this->nbrclient]->nickname = this->cmd[1];
        this->clientList[this->nbrclient]->Nickname_Status = 1;
        /* The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.*/
        return ;
    }
}
void   Server::user()
{
    if (this->cmd.size() - 1 < 4)
          std::cout << ERR_NEEDMOREPARAMS << std::endl;
    if (this->clientList[this->nbrclient]->Nickname_Status == 0) // doit etre change qu'une fois.
    {
        if (!this->clientList[this->nbrclient]->Password_Status || !this->clientList[this->nbrclient]->Nickname_Status)
            return (std::cout << "ERR_NEEDPASSWORDBEFORE" << std::endl, (void)0); // A changer ?
        // if (this->isprint(this->cmd[1])) // ????
        //      return (std::cout << ERR_ERRONEUSNICKNAME << std::endl, (void)0);  
        if (this->cmd[1].size() > 9 || this->cmd[1].size() < 1)
            return (std::cout << ERR_ERRONEUSNICKNAME << std::endl, (void)0);
        if (this->cmd[4].size() > 9 || this->cmd[4].size() < 1)
            return (std::cout << ERR_ERRONEUSNICKNAME << std::endl, (void)0);
        
        this->clientList[this->nbrclient]->username = this->cmd[1];
        this->clientList[this->nbrclient]->Username_Status = 1;
        this->clientList[this->nbrclient]->isRegistered = 1;
        if (this->clientList[this->nbrclient]->Password_Status == -1) // wrong password
            return this->closeClient("ERR_PASSWDMISMATCH");
    }
    else
        std::cout << ERR_ALREADYREGISTRED << std::endl;
    // join j'ai enlever le epollout. etc etc
}