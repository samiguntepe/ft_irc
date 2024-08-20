#include "../includes/Server.hpp"

void Server::removeChannel(const std::string& channelName)
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

void Server::removeClientFromAllChannels(Client* client)
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
			string channelName = channel->get_channel_name();
			string clientNick = client->getNickName();
			channel->removeUserFromChannel(client);
			client->removeChannel(channel);
			string leaveMessage = clientNick + " has left the channel " + channelName;
			log(leaveMessage);
			if (channel->get_channel_client_count() == 0 && channelExists(channel->get_channel_name()))
			{
				string message = "Channel " + channelName + " is empty, deleting.\n";
				write(STDOUT_FILENO, message.c_str(), message.size());
				removeChannel(channelName);
			}
		}
	}
}

void Server::addChannel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->get_channel_name(), channel));
}

bool Server::channelExists(const string& channelName)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_channel_name() == channelName)
			return true;
	}
	return false;
}

Client* Server::getClient(string& nickName)
{
	for (map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickName() == nickName)
			return it->second;
	}
	return NULL;
}

Channel* Server::getChannel(string& channelName)
{
	for (map<string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->get_channel_name() == channelName)
			return it->second;
	}
	return NULL;
}
