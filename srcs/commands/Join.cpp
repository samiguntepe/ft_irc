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

void Join::joinChannel(Client* client, string channel_name, vector<string> commandParts, Server* srv)
{
    string channelPass;
    if (channel_name.empty())
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channel_name));
        return;
    }
    if (commandParts.size() == 2)
        channelPass = "";
    else
        channelPass = commandParts.at(2);
    if (channel_name.size() < 2 || channel_name.at(0) != '#')
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channel_name));
        return;
    }
    Channel* channel;
    if (srv->channel_exists(channel_name))
    {
        channel = srv->getChannel(channel_name);
        if (channel->isUserOnChannel(client))
        {
            client->sendReply(ERR_USERONCHANNEL(client->getNickName(), channel_name));
            return;
        }
        else if (channel->getChannelKey() != channelPass)
        {
            client->sendReply(ERR_BADCHANNELKEY(client->getNickName(), channel_name));
            return;
        }
        else if (channel->getUserLimit() <= channel->getChannelClientCount())
        {
            client->sendReply(ERR_CHANNELISFULL(client->getNickName(), channel_name));
            return;
        }
    }
    else
    {
        channel = new Channel(channel_name, channelPass, client);
        channel->setChannelOwner(client);
        client->setOperator(true);
        srv->addChannel(channel);
        channel->setNoExternalMessages(true);
		channel->broadcastMessage("MODE " + channel->getchannel_name() + " +n " + client->getNickName());
    }
    client->join(channel);
}
