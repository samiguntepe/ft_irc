#include "../includes/Server.hpp"

void Server::remove_channel(const std::string& channel_name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
	if (it != _channels.end())
	{
		Channel* channel = it->second;
		if (channel != NULL)
		{
			delete channel;
			_channels.erase(it);
		}
		else
		{
			string message = "Channel " + channel_name + " is already null.";
			write(STDOUT_FILENO, message.c_str(), message.size());
		}
	}
	else
	{
		string message = "Channel " + channel_name + " does not exist.";
		write(STDOUT_FILENO, message.c_str(), message.size());
	}
}

void Server::remove_client_all_channels(Client* client)
{
	if (client == NULL || !client->isSocketOpen())
		return;
	std::vector<Channel*> channels = client->getChannel();
	while (!channels.empty())
	{
		Channel* channel = channels.back();
		channels.pop_back();
		if (channel != NULL)
		{
			string channel_name = channel->getchannel_name();
			string clientNick = client->getNickName();
			channel->removeUserFromChannel(client);
			client->remove_channel(channel);
			string leaveMessage = clientNick + " has left the channel " + channel_name;
			log(leaveMessage);
			if (channel->getChannelClientCount() == 0 && channel_exists(channel->getchannel_name()))
			{
				string message = "Channel " + channel_name + " is empty, deleting.\n";
				write(STDOUT_FILENO, message.c_str(), message.size());
				remove_channel(channel_name);
			}
		}
	}
}

void Server::addChannel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->getchannel_name(), channel));
}

bool Server::channel_exists(const string& channel_name)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->getchannel_name() == channel_name)
			return true;
	}
	return false;
}

Client* Server::get_client(string& nickName)
{
	for (map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNickName() == nickName)
			return it->second;
	}
	return NULL;
}

Channel* Server::getChannel(string& channel_name)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->getchannel_name() == channel_name)
			return it->second;
	}
	return NULL;
}
