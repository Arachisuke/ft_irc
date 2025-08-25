#include "Client.hpp"
#include "Server.hpp"

void   Server::ping()
{
    if (this->cmd.size() - 1 == 0)
            return(std::cout << "ERR_NEEDMOREPARAMS" << std::endl, (void)0);
    this->PushMsg("HuecoMundo: PONG" + this->cmd[1]);
}