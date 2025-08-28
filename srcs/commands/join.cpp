#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

int Server::findChannel(std::string channel)
{
    for (size_t i = 0; i < this->channeList.size(); i++)
    {
        if (this->channeList[i]->getName() == channel)
            return (i);
    }
    return (-1);
}

int Server::imInOrNot(std::string channel)
{
    int channelIndex = findChannel(channel);
    if (channelIndex == -1)
        return (0);
    
    // Recherche dans le set
    for (std::set<Client *>::iterator it = this->channeList[channelIndex]->getUsers().begin(); 
         it != this->channeList[channelIndex]->getUsers().end(); ++it)
    {
        if ((*it)->nickname == this->clientList[this->nbrclient]->nickname)
            return (1);
    }
    return (0);
}


void   Server::join()
{
    if (this->cmd.size() - 1 == 0)
            return(std::cout << "ERR_NEEDMOREPARAMS" << std::endl, (void)0);
    if (this->clientList[this->nbrclient]->isRegistered == 0)
        return (std::cout << "ERR_NOTREGISTRED" << std::endl, (void)0);
    if (this->cmd[1][0] != '#')
        return (std::cout << "ERR_NOSUCHCHANNEL" << std::endl, (void)0);

    if (findChannel(this->cmd[1]) != -1) // channel exist
    {
        int i = findChannel(this->cmd[1]);
        if (this->channeList[i]->getModes('i')) // invite only
            return (std::cout << "ERR_INVITEONLYCHAN" << std::endl, (void)0);
        if (this->channeList[i]->getModes('k')) // key only
            return (std::cout << "ERR_BADCHANNELKEY" << std::endl, (void)0);
        if (this->channeList[i]->getModes('l')) // limit only
            return (std::cout << "ERR_CHANNELISFULL" << std::endl, (void)0);
        if (this->channeList[i]->getModes('o'))  // a changer
            return (std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl, (void)0);
        if (this->channeList[i]->getModes('t')) // a changer 
            return (std::cout << "ERR_BANNEDFROMCHAN" << std::endl, (void)0);
        if (imInOrNot(this->cmd[1])) 
            return (std::cout << "ERR_USERONCHANNEL" << std::endl, (void)0);
        this->channeList[i]->setUsers(this->clientList[this->nbrclient]);
        // this->clientList[this->nbrclient]->listofchannel.push_back(this->cmd[1]);
    }
}
                                  
      /*
        
        etape 0; verifie si je peux accede au channel selon son mode et si je suis pas deja dedans.
        etape 1; integrer a la class client lui dire qu'il est bien dans le channel via un int ou un bool.
        etape 2; integrer a la class channel lui dire qu'il y a un nouveau membre dans sa listofmembre.
        etape 3; envoyer a tout les membres du channel le message de join.
        etape 4; envoyer au client qui join le topic du channel.
        etape 5; envoyer au client qui join la listofmembre du channel.
        etape 6; envoyer au client qui join le mode du channel.
      */
    // else
    // {
    //     Channel *newChannel = new Channel();
    //     newChannel->name = this->cmd[1];
    //     this->channeList.push_back(newChannel);
        // etape 1; integrer a la class client lui dire qu'il est bien dans le channel via un int ou un bool.
        // etape 2; integrer a la class channel lui dire qu'il y a un nouveau membre dans sa listofmembre.
        // etape 3; envoyer a tout les membres du channel le message de join.
        // etape 4; envoyer au client qui join le topic du channel.
        // etape 5; envoyer au client qui join la listofmembre du channel.
        // etape 6; envoyer au client qui join le mode du channel.
        // etape 7; envoyer au client qui join le limit du channel.
        // etape 8; envoyer au client qui join le key du channel.
        // etape 9; envoyer au client qui join le invite du channel.
        // etape 10; envoyer au client qui join le ban du channel.
        // etape 11; envoyer au client qui join le exception du channel.
      // le rendre operateur du channel.
    
        
  // they receive all relevant information about that channel including the JOIN, PART, KICK, and MODE messages affecting the channel
  // if successfull, <client> nameofchannel 
  // RPLTOPIC MSG.
  // all client membre of the channel en comptant le client qui viens de join.
  // RPLNAMREPLY
  // RPL_ENDOFNAMES
  // modeofchannel a implementer ici.
  // limitofchannel ?
  




//channel1 channel2   key1 key2