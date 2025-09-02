#include "../../header/Client.hpp"
#include "../../header/Server.hpp"
#include "../../header/Channel.hpp"
int Server::findChannel(std::string channel)
{
    for (size_t i = 0; i < this->channeList.size(); i++)
    {
        if (this->channeList[i]->getName() == channel)
            return (i);
    }
    return (-1);
}

int Server::whereIsChannel(std::string channel)
{
    for (size_t i = 0; i < this->clientList[this->nbrclient]->listofchannel.size(); i++)
    {
        if (this->clientList[this->nbrclient]->listofchannel[i]->getName() == channel)
            return (i);
    }
    return (-1);
}

void Server::successfullJoin(int i)
{
    std::set<Client *> users = this->channeList[i]->getUsers();
    for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        std::string msg = ":" + this->clientList[this->nbrclient]->nickname + "!" + this->clientList[this->nbrclient]->username + "@localhost" + " JOIN " + this->channeList[i]->getName() + "\r\n";
        send((*it)->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
    }
    if (this->channeList[i]->getTopic() != "")
        std::string topic = ":" + this->_serverName + " 332 " + this->clientList[this->nbrclient]->nickname + this->channeList[i]->getName() + " :" + this->channeList[i]->getTopic();
    std::string msg = ":" + this->_serverName + " 353 " + this->clientList[this->nbrclient]->nickname + " = " + this->channeList[i]->getName() + " : ";
    for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++) // afficher les membres
        msg += (*it)->nickname + " ";
    msg += "\r\n";
    send(this->clientList[this->nbrclient]->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
    std::string msg2 = ":" + this->_serverName + " 366 " + this->clientList[this->nbrclient]->nickname + " " + this->channeList[i]->getName() + " :End of /NAME list" + "\r\n";
    send(this->clientList[this->nbrclient]->fd, msg2.c_str(), msg2.size(), MSG_DONTWAIT);
}

void Server::join()
{
    std::vector<std::string> list;
    if (this->cmd.size() - 1 == 0)
        return(reply(461, "JOIN", "Not Enough Parameters", *this->clientList[this->nbrclient]), (void)0);
    if (this->clientList[this->nbrclient]->isRegistered == 0)
        return(reply(451, "JOIN", "You have not registered", *this->clientList[this->nbrclient]), (void)0);

    list = ft_split(this->cmd[1], ',');
    for (size_t j = 0; j < list.size(); j++)
    {
        if (list[j][0] != '#')
            return(reply(403, "JOIN", ERR_NOSUCHCHANNEL, *this->clientList[this->nbrclient]), (void)0);

        if (findChannel(list[j]) != -1) // channel exist
        {
            int i = findChannel(list[j]);
            if (this->channeList[i]->getModes('i')) // invite only
                return (std::cout << "ERR_INVITEONLYCHAN" << std::endl, (void)0);
            if (this->channeList[i]->getModes('k')) // key only
                return (std::cout << "ERR_BADCHANNELKEY" << std::endl, (void)0);
            if (this->channeList[i]->getModes('l')) // limit only
                return (std::cout << "ERR_CHANNELISFULL" << std::endl, (void)0);
            if (this->channeList[i]->getModes('o')) // a changer
                return (std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl, (void)0);
            if (this->channeList[i]->getModes('t')) // a changer
                return (std::cout << "ERR_BANNEDFROMCHAN" << std::endl, (void)0);
            if (this->channeList[i]->isMember(this->clientList[this->nbrclient]))
            {
                std::string msg = ":" + this->clientList[this->nbrclient]->nickname + "!" + this->clientList[this->nbrclient]->username + "@localhost" + " JOIN " + this->channeList[i]->getName() + " is already on channel" + "\r\n";
                send(this->clientList[this->nbrclient]->fd, msg.c_str(), msg.size(), MSG_DONTWAIT); // a verifie car c'est que pour NICK ce msg.
            }
            this->channeList[i]->addClient(this->clientList[this->nbrclient]);
            this->clientList[this->nbrclient]->listofchannel.push_back(this->channeList[i]);
            this->successfullJoin(i);
        }
        else // donne les droits.
        {
            Channel *newChannel = new Channel();
            newChannel->setName(list[j]);
            this->channeList.push_back(newChannel);
            this->channeList[this->channeList.size() - 1]->addClient(this->clientList[this->nbrclient]);
            this->clientList[this->nbrclient]->listofchannel.push_back(newChannel);
            this->channeList[this->channeList.size() - 1]->addOperator(this->clientList[this->nbrclient]);
            this->channeList[this->channeList.size() - 1]->setTopicSetter(this->clientList[this->nbrclient]->nickname);
            this->channeList[this->channeList.size() - 1]->setTopic("");
            this->successfullJoin(this->channeList.size() - 1);
        }
    }
   
    if (this->cmd[1] == "0")
    {
        std::vector<Channel *> &list = this->clientList[this->nbrclient]->listofchannel;
        for (size_t i = 0; i < list.size(); i++)
            {
               if (!list[i]->isMember(this->clientList[this->nbrclient]))
                   return(reply(442, "PART", "You're not on that channel", *this->clientList[this->nbrclient]), (void)0);
               std::string msg = ":" + this->clientList[this->nbrclient]->nickname + "!" + this->clientList[this->nbrclient]->username + "@localhost" + " PART " + list[i]->getName();
               if (this->cmd[2] != "")
                   msg += " :" + this->cmd[2];
              msg += "\r\n";
               std::set<Client *> users = list[i]->getUsers();
               for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
                   send((*it)->fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
               list[i]->removeClient(this->clientList[this->nbrclient]);
               if (list[i]->getUsers().empty())
                   delete this->channeList[i];
                this->channeList.erase(this->channeList.begin() + i);
               int b = whereIsChannel(list[i]->getName());
                this->clientList[this->nbrclient]->listofchannel.erase(this->clientList[this->nbrclient]->listofchannel.begin() + b);
            }
    }
       
}

