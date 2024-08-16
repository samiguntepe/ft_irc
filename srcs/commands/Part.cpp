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
        string channel_name;
        while (getline(iss, channel_name, ','))
            partChannel(client, channel_name, srv);
    }
}

void Part::partChannel(Client* client, string channel_name, Server* srv)
{
    if (channel_name.empty() || channel_name.at(0) != '#')
    {
      	client->send_message( ":" + client->get_host_name() + " 403 " + client->get_nick_name() + " " + channel_name + " :No such channel" );
        return;
    }
    Channel* channel = srv->get_channel(channel_name);
    if (!channel)
    {
     	client->send_message( ":" + client->get_host_name() + " 403 " + client->get_nick_name() + " " + channel_name + " :No such channel" );
        return;
    }
    if (!channel->is_user_on_channel(client))
    {
        client->send_message( ":" + client->get_host_name() + " 442 " + client->get_nick_name() + " " + channel_name + " :You're not on that channel" );
        return;
    }
    channel->remove_user_from_channel(client);
    client->remove_channel(channel);

    string message = ":" + client->get_prefix() + " PART " + channel_name + "\n";
    channel->broadcast_message(message, client);

   	client->send_message( "You left the channel " + channel_name );
    if (channel->get_channel_client_count() == 0 && srv->channel_exists(channel_name))
    {
   		string channel_name = channel->get_channel_name();
		string message = "Channel " + channel_name + " is empty, deleting.\n";
		write( 1, message.c_str(), message.length() );
		srv->remove_channel(channel_name);
    }
}
