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

int Server::whereIsChannel(std::string channel)
{
    for (size_t i = 0; i < this->_clientList[this->_nbrclient]->getlistofchannel().size(); i++)
    {
        if (this->_clientList[this->_nbrclient]->getlistofchannel()[i]->getName() == channel)
            return (i);
    }
    return (-1);
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
        std::string topic = ":" + this->_serverName + " 332 " + this->_clientList[this->_nbrclient]->getNickname() + this->_channeList[i]->getName() + " :" + this->_channeList[i]->getTopic();
    std::string msg = ":" + this->_serverName + " 353 " + this->_clientList[this->_nbrclient]->getNickname() + " = " + this->_channeList[i]->getName() + " : ";
    for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++) // afficher les membres
        msg += (*it)->getNickname() + " ";
    msg += "\r\n";
    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
    std::string msg2 = ":" + this->_serverName + " 366 " + this->_clientList[this->_nbrclient]->getNickname() + " " + this->_channeList[i]->getName() + " :End of /NAME list" + "\r\n";
    send(this->_clientList[this->_nbrclient]->getFd(), msg2.c_str(), msg2.size(), MSG_DONTWAIT);
}

void Server::join()
{
    if (this->_cmd.size() - 1 == 0)
        return(reply(461, "JOIN", "Not Enough Parameters", *this->_clientList[this->_nbrclient]), (void)0);
    if (this->_clientList[this->_nbrclient]->getisRegistered() == 0)
        return(reply(451, "JOIN", "You have not registered", *this->_clientList[this->_nbrclient]), (void)0);

    if (this->_cmd[1] == "0")
    {
            std::vector<Channel *> list = this->_clientList[this->_nbrclient]->getlistofchannel();
            for (size_t i = 0; i < list.size(); i++)
                {
                    std::cout << "JOIN : 0 nameofchannel " << list[i]->getName() << std::endl;
                    std::cout << list.size() << std::endl;
                   if (!list[i]->isMember(this->_clientList[this->_nbrclient]))
                       return(reply(442, "JOIN", "You're not on that channel", *this->_clientList[this->_nbrclient]), (void)0);
                   std::string msg = ":" + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " JOIN " + list[i]->getName();
                    if (this->_cmd.size() > 2 && this->_cmd[2] != "")
                        msg += " : " + this->_cmd[2];
                    msg += "\r\n";
                   std::set<Client *> users = list[i]->getUsers();
                   for (std::set<Client *>::iterator it = users.begin(); it != users.end(); it++)
                       send((*it)->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT); // je change le msg ? car cest ecris join ca peut preter a confusions ?
                    int b = whereIsChannel(list[i]->getName());
                    this->_clientList[this->_nbrclient]->setListOfchannel().erase(this->_clientList[this->_nbrclient]->setListOfchannel().begin() + b);
                    int x = findChannel(list[i]->getName());
                    this->_channeList[x]->removeClient(this->_clientList[this->_nbrclient]);
                   if (list[i]->getUsers().empty())
                       delete this->_channeList[x];
                    this->_channeList.erase(this->_channeList.begin() + i);
                }
    }
    else
    {
       std::vector<std::string> list = ft_split(this->_cmd[1], ',');
        for (size_t j = 0; j < list.size(); j++)
        {
            if (list[j][0] != '#')
                return(reply(403, "JOIN", ERR_NOSUCHCHANNEL, *this->_clientList[this->_nbrclient]), (void)0);
    
            if (findChannel(list[j]) != -1) // channel exist
            {
                int i = findChannel(list[j]);
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
                if (this->_channeList[i]->isMember(this->_clientList[this->_nbrclient]))
                {
                    std::string msg = ":" + this->_serverName + " 443 " + this->_clientList[this->_nbrclient]->getNickname() + "!" + this->_clientList[this->_nbrclient]->getUsername() + "@localhost" + " JOIN " + this->_channeList[i]->getName() + " is already on channel" + "\r\n";
                    send(this->_clientList[this->_nbrclient]->getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
                }
                this->_channeList[i]->setUsers(this->_clientList[this->_nbrclient]);
                this->_clientList[this->_nbrclient]->setListOfchannel().push_back(this->_channeList[i]);
                this->successfullJoin(i);
            }
            else // les droits.
            {
                std::cout << "JOIN basique : nameofchannel " << list[j] << std::endl;
                Channel *newChannel = new Channel();
                newChannel->setName(list[j]);
                this->_channeList.push_back(newChannel);
                this->_channeList[this->_channeList.size() - 1]->setUsers(this->_clientList[this->_nbrclient]);
                this->_clientList[this->_nbrclient]->setListOfchannel().push_back(newChannel);
                this->_channeList[this->_channeList.size() - 1]->addOperator(this->_clientList[this->_nbrclient]);
                this->_channeList[this->_channeList.size() - 1]->setTopicSetter(this->_clientList[this->_nbrclient]->getNickname());
                this->_channeList[this->_channeList.size() - 1]->setTopic("");
                this->successfullJoin(this->_channeList.size() - 1);
            }
        }
    }      
}

