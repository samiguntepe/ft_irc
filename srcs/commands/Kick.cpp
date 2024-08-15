#include "../../includes/Commands.hpp"

void Kick::kick(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 3)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "KICK"));
        return;
    }
    string channel_namee = commandParts.at(1);
    string nickName = commandParts.at(2);
    string reason = (commandParts.size() > 3) ? commandParts.at(3) : "No reason specified";
    if (channel_namee.at(0) != '#')
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channel_namee));
        return;
    }
    Channel* channel = srv->getChannel(channel_namee);
    if (!channel)
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channel_namee));
        return;
    }
    if (!channel->isUserOnChannel(client))
    {
        client->sendReply(ERR_NOTONCHANNEL(client->getNickName(), channel_namee));
        return;
    }
    Client* target = srv->get_client(nickName);
    if (!target)
    {
        client->sendReply(ERR_NOSUCHNICK(client->getNickName(), nickName));
        return;
    }
    if (!channel->isUserOnChannel(target))
    {
        client->sendReply(ERR_USERNOTINCHANNEL(client->getNickName(), nickName, channel_namee));
        return;
    }
    if (channel->getChannelOwner()->getNickName() == target->getNickName())
    {
        client->sendReply(ERR_NOKICKCHANNELOWNER(client->getNickName(), channel_namee));
        return;
    }
    if (client->isOperator())
    {
        channel->removeUserFromChannel(target);
        target->remove_channel(channel);
        string message = ":" + client->getPrefix() + " KICK " + channel_namee + " " + nickName + " :" + reason + "\n";
        channel->broadcastMessage(message, client);
        target->sendMessage(":" + client->getPrefix() + " KICK " + channel_namee + " " + nickName + " :" + reason);
        target->sendMessage("You have been kicked from channel " + channel_namee + " by " + client->getNickName());
        string leavemessage = client->getNickName() + " kicked " + target->getNickName() + " from channel " + channel->getchannel_name();
        log(leavemessage);
    }
    else
    {
        client->sendReply(ERR_CHANOPRIVSNEEDED(client->getNickName(), channel_namee));
        return;
    }
    if (channel->getChannelClientCount() == 0)
    {
        string channel_name = channel->getchannel_name();
        srv->remove_channel(channel_name);
    }
}
