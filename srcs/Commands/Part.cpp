#include "../../includes/Commands.hpp"

void Part::part(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 2)
    {
        client->send_message( ":" + client->get_host_name() + " 461 " + client->get_nick_name() + " PART : Not enough parameters" );
        return;
    }
    string target = commandParts.at(1);
    if (target.find(',') == string::npos)
        partChannel(client, target, srv);
    else
    {
        istringstream iss(target);
        string channelName;
        while (getline(iss, channelName, ','))
            partChannel(client, channelName, srv);
    }
}

void Part::partChannel(Client* client, string channelName, Server* srv)
{
    if (channelName.empty() || channelName.at(0) != '#')
    {
      	client->send_message( ":" + client->get_host_name() + " 403 " + client->get_nick_name() + " " + channelName + " :No such channel" );
        return;
    }
    Channel* channel = srv->get_channel(channelName);
    if (!channel)
    {
     	client->send_message( ":" + client->get_host_name() + " 403 " + client->get_nick_name() + " " + channelName + " :No such channel" );
        return;
    }
    if (!channel->is_user_on_channel(client))
    {
        client->send_message( ":" + client->get_host_name() + " 442 " + client->get_nick_name() + " " + channelName + " :You're not on that channel" );
        return;
    }
    channel->remove_user_from_channel(client);
    client->remove_channel(channel);

    string message = ":" + client->get_prefix() + " PART " + channelName + "\n";
    channel->broadcast_message(message, client);

   	client->send_message( "You left the channel " + channelName );
    if (channel->get_channel_client_count() == 0 && srv->channel_exists(channelName))
    {
   		string channelName = channel->get_channel_name();
		string message = "Channel " + channelName + " is empty, deleting.\n";
		write( 1, message.c_str(), message.length() );
		srv->remove_channel(channelName);
    }
}
