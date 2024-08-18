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
			std::cout << "Channel " << channel_name << " is already null.";
		}
	}
	else
	{
		std::cout << "Channel " << channel_name << " does not exist.";
	}
}

void Server::remove_client_all_channels(Client* client)
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
			string channel_name = channel->get_channel_name();
			string clientNick = client->get_nick_name();
			channel->remove_user_from_channel(client);
			client->remove_channel(channel);
			string leaveMessage = clientNick + " has left the channel " + channel_name;
			log(leaveMessage);
			if (channel->get_channel_client_count() == 0 && channel_exists(channel->get_channel_name()))
			{
				std::cout << "Channel " << channel_name << " is empty, deleting."<< std::endl;
				remove_channel(channel_name);
			}
		}
	}
}

void Server::addChannel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->get_channel_name(), channel));
}

bool Server::channel_exists(const string& channel_name)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_channel_name() == channel_name)
			return true;
	}
	return false;
}

Client* Server::get_client(string& nickName)
{
	for (map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->get_nick_name() == nickName)
			return it->second;
	}
	return NULL;
}

Channel* Server::get_channel(string& channel_name)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_channel_name() == channel_name)
			return it->second;
	}
	return NULL;
}
