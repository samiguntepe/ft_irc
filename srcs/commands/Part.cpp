#include "../../includes/Commands.hpp"

void Part::part(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 2)
    {
        client->sendMessage( ":" + client->getHostName() + " 461 " + client->getNickName() + " PART : Not enough parameters" );
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
      	client->sendMessage( ":" + client->getHostName() + " 403 " + client->getNickName() + " " + channel_name + " :No such channel" );
        return;
    }
    Channel* channel = srv->getChannel(channel_name);
    if (!channel)
    {
     	client->sendMessage( ":" + client->getHostName() + " 403 " + client->getNickName() + " " + channel_name + " :No such channel" );
        return;
    }
    if (!channel->isUserOnChannel(client))
    {
        client->sendMessage( ":" + client->getHostName() + " 442 " + client->getNickName() + " " + channel_name + " :You're not on that channel" );
        return;
    }
    channel->removeUserFromChannel(client);
    client->remove_channel(channel);

    string message = ":" + client->getPrefix() + " PART " + channel_name + "\n";
    channel->broadcastMessage(message, client);

   	client->sendMessage( "You left the channel " + channel_name );
    if (channel->getChannelClientCount() == 0 && srv->channel_exists(channel_name))
    {
   		string channel_name = channel->getchannel_name();
		string message = "Channel " + channel_name + " is empty, deleting.\n";
		write( 1, message.c_str(), message.length() );
		srv->remove_channel(channel_name);
    }
}
