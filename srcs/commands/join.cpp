#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"
int Server::findChannel(std::string channel)
{
    for (size_t i = 0; i < this->_channeList.size(); i++)
    {
        if (this->_channeList[i]->getName() == channel)
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
    for (std::set<Client *>::iterator it = this->_channeList[channelIndex]->getUsers().begin();
         it != this->_channeList[channelIndex]->getUsers().end(); ++it)
    {
        if ((*it)->getNickname() == this->_clientList[this->_nbrclient]->getNickname())
            return (1);
    }
    return (0);
}

void Server::successfullJoin(int i)
{
    std::set<Client *> users = this->_channeList[i]->getUsers();
    for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " JOIN " + this->_channeList[i]->getName() + "\r\n";
        send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
    }
    if (this->_channeList[i]->getTopic() != "")
        std::string topic = ":" + this->_serverName + "332" + this->_clientList[this->_nbrclient]->getNickname() + this->_channeList[i]->getName() + " :" + this->_channeList[i]->getTopic();
    std::string msg = ":" + this->_serverName + "353" + this->_clientList[this->_nbrclient]->getNickname() + " = " + this->_channeList[i]->getName() + " : ";
    for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        msg += (*it)->getNickname() + " ";
    }
    msg += "\r\n";
    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
    std::string msg2 = ":" + this->_serverName + "366" + this->_clientList[this->_nbrclient]->getNickname() + this->_channeList[i]->getName() + " :End of /NAME list";
    send(this->_clientList[this->_nbrclient]->getFd(), msg2.c_str(), msg2.size(), MSG_DONTWAIT);
}

void Server::join()
{
    if (this->_cmd.size() - 1 == 0)
        return (std::cout << "ERR_NEEDMOREPARAMS" << std::endl, (void)0);
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
        return (std::cout << "ERR_NOTREGISTRED" << std::endl, (void)0);
    if (this->_cmd[1][0] != '#')
        return (std::cout << "ERR_NOSUCHCHANNEL" << std::endl, (void)0);

    if (findChannel(this->_cmd[1]) != -1) // channel exist
    {
        int i = findChannel(this->_cmd[1]);
        if (this->_channeList[i]->getModes('i')) // invite only
            return (std::cout << "ERR_INVITEONLYCHAN" << std::endl, (void)0);
        if (this->_channeList[i]->getModes('k')) // key only
            return (std::cout << "ERR_BADCHANNELKEY" << std::endl, (void)0);
        if (this->_channeList[i]->getModes('l')) // limit only
            return (std::cout << "ERR_CHANNELISFULL" << std::endl, (void)0);
        if (this->_channeList[i]->getModes('o')) // a changer
            return (std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl, (void)0);
        if (this->_channeList[i]->getModes('t')) // a changer
            return (std::cout << "ERR_BANNEDFROMCHAN" << std::endl, (void)0);
        if (imInOrNot(this->_cmd[1]))
            return (std::cout << "ERR_USERONCHANNEL" << std::endl, (void)0);
        this->_channeList[i]->setUsers(this->_clientList[this->_nbrclient]);
        this->successfullJoin(i);
    }
    else // donne les droits.
    {
        Channel *newChannel = new Channel();
        newChannel->setName(this->_cmd[1]);
        this->_channeList.push_back(newChannel);
        this->successfullJoin(this->_channeList.size() - 1);
        this->_channeList[this->_channeList.size() - 1]->setUsers(this->_clientList[this->_nbrclient]);
        this->_channeList[this->_channeList.size() - 1]->addOperator(this->_clientList[this->_nbrclient]);
        this->_channeList[this->_channeList.size() - 1]->setTopicSetter(this->_clientList[this->_nbrclient]->getNickname());
        this->_channeList[this->_channeList.size() - 1]->setTopic("");
    }
    if (this->_cmd[2] == "0")
        ;
    // PART supprime le client de tout les channels.
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

// channel1 channel2   key1 key2