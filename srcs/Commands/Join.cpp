#include "../../includes/Commands.hpp"

void Join::join(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 2)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "JOIN"));
        return;
    }
    string channel = commandParts.at(1);
    if (!channel.empty())
        joinChannel(client, channel, commandParts, srv); 
}

void Join::joinChannel(Client* client, string channelName, vector<string> commandParts, Server* srv)
{
    string channelPass;
    if (channelName.empty())
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }
    if (commandParts.size() == 2)
        channelPass = "";
    else
        channelPass = commandParts.at(2);
    if (channelName.size() < 2 || channelName.at(0) != '#')
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
        return;
    }
    Channel* channel;
    if (srv->channelExists(channelName))
    {
        channel = srv->getChannel(channelName);
        if (channel->isUserOnChannel(client))
        {
            client->sendReply(ERR_USERONCHANNEL(client->getNickName(), channelName));
            return;
        }
        else if (channel->getChannelKey() != channelPass)
        {
            client->sendReply(ERR_BADCHANNELKEY(client->getNickName(), channelName));
            return;
        }
        else if (channel->getUserLimit() <= channel->getChannelClientCount())
        {
            client->sendReply(ERR_CHANNELISFULL(client->getNickName(), channelName));
            return;
        }
    }
    else
    {
        channel = new Channel(channelName, channelPass, client);
        channel->setChannelOwner(client);
        client->setOperator(true);
        srv->addChannel(channel);
        channel->setNoExternalMessages(true);
		channel->broadcastMessage("MODE " + channel->getChannelName() + " +n " + client->getNickName());
    }
    client->join(channel);
}
