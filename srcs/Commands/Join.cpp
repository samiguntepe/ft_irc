#include "../../includes/Commands.hpp"

void Join::join(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 2)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "JOIN"));
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
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channelName));
        return;
    }
    if (commandParts.size() == 2)
        channelPass = "";
    else
        channelPass = commandParts.at(2);
    if (channelName.size() < 2 || channelName.at(0) != '#')
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channelName));
        return;
    }
    Channel* channel;
    if (srv->channel_exists(channelName))
    {
        channel = srv->get_channel(channelName);
        if (channel->is_user_on_channel(client))
        {
            client->send_reply(ERR_USERONCHANNEL(client->get_nick_name(), channelName));
            return;
        }
        else if (channel->get_channel_key() != channelPass)
        {
            client->send_reply(ERR_BADCHANNELKEY(client->get_nick_name(), channelName));
            return;
        }
        else if (channel->get_user_limit() <= channel->get_channel_client_count())
        {
            client->send_reply(ERR_CHANNELISFULL(client->get_nick_name(), channelName));
            return;
        }
    }
    else
    {
        channel = new Channel(channelName, channelPass, client);
        channel->set_channel_owner(client);
        client->set_operator(true);
        srv->add_channel(channel);
        channel->set_no_external_messages(true);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " +n " + client->get_nick_name());
    }
    client->join(channel);
}
