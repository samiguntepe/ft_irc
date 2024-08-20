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
        if (channel->is_user_on_channel(client))
        {
            client->sendReply(ERR_USERONCHANNEL(client->getNickName(), channelName));
            return;
        }
        else if (channel->get_channel_key() != channelPass)
        {
            client->sendReply(ERR_BADCHANNELKEY(client->getNickName(), channelName));
            return;
        }
        else if (channel->get_user_limit() <= channel->get_channel_client_count())
        {
            client->sendReply(ERR_CHANNELISFULL(client->getNickName(), channelName));
            return;
        }
    }
    else
    {
        channel = new Channel(channelName, channelPass, client);
        channel->set_channel_owner(client);
        client->setOperator(true);
        srv->addChannel(channel);
        channel->set_no_external_messages(true);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " +n " + client->getNickName());
    }
    client->join(channel);
}
