#include "../includes/Server.hpp"

void Server::remove_channel(const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
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
			string message = "Channel " + channelName + " is already null.";
			write(STDOUT_FILENO, message.c_str(), message.size());
		}
	}
	else
	{
		string message = "Channel " + channelName + " does not exist.";
		write(STDOUT_FILENO, message.c_str(), message.size());
	}
}

void Server::remove_client_from_all_channels(Client* client)
{
	if (client == NULL || !client->is_socket_open())
		return;
	std::vector<Channel*> channels = client->get_channel();
	while (!channels.empty())
	{
		Channel* channel = channels.back();
		channels.pop_back();
		if (channel != NULL)
		{
			string channelName = channel->get_channel_name();
			string clientNick = client->get_nick_name();
			channel->remove_user_from_channel(client);
			client->remove_channel(channel);
			if (channel->get_channel_client_count() == 0 && channel_exists(channel->get_channel_name()))
			{
				string message = "Channel " + channelName + " is empty, deleting.\n";
				write(STDOUT_FILENO, message.c_str(), message.size());
				remove_channel(channelName);
			}
		}
	}
}

void Server::add_channel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->get_channel_name(), channel));
}

bool Server::channel_exists(const string& channelName)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_channel_name() == channelName)
			return true;
	}
	return false;
}

Client* Server::get_client(string& nickName)
{
	for (map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->get_nick_name() == nickName)
			return it->second;
	}
	return NULL;
}

Channel* Server::get_channel(string& channelName)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_channel_name() == channelName)
			return it->second;
	}
	return NULL;
}
