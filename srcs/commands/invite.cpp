#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Channel *Server::findChannel(std::string &channelName)
{
    std::map<std::string, Channel *>::const_iterator it = channelList.find(channelName);
    if (it != channelList.end())
        return (it->second);
    return (NULL);
}

void Server::invite()
{
    if (!clientList[nbrclient]->isRegistered)
    {
        // error not registered
    }
    if (cmd.size() < 2)
    {
        // error not enough params
    }
    int clientIndex = find_client(cmd[0]);
    Channel *channel = findChannel(cmd[1]);
    if (clientList[nbrclient]->nickname == clientList[clientIndex]->nickname || clientIndex == -1)
    {
        // error client introuvable ou host lui meme
    }
    if (channel)
    {
        if (!channel->getIsMember(*clientList[nbrclient]))
        // error client qui invite n est pas dans le channel
        if (channel->getOnInviteOnly() && channel->getIsOperator(*clientList[nbrclient]))
        // error invite only et non operator
        if (channel->getIsMember(*clientList[clientIndex]))
        // error guest deja sur la channel
        channel->inviteMember(clientList[clientIndex]);
    }
}
