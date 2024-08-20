#include "../../includes/Commands.hpp"

void Kick::kick(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 3)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "KICK"));
        return;
    }
    string channelNamee = commandParts.at(1);
    string nickName = commandParts.at(2);
    string reason = (commandParts.size() > 3) ? commandParts.at(3) : "No reason specified";
    if (channelNamee.at(0) != '#')
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channelNamee));
        return;
    }
    Channel* channel = srv->getChannel(channelNamee);
    if (!channel)
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channelNamee));
        return;
    }
    if (!channel->is_user_on_channel(client))
    {
        client->sendReply(ERR_NOTONCHANNEL(client->getNickName(), channelNamee));
        return;
    }
    Client* target = srv->getClient(nickName);
    if (!target)
    {
        client->sendReply(ERR_NOSUCHNICK(client->getNickName(), nickName));
        return;
    }
    if (!channel->is_user_on_channel(target))
    {
        client->sendReply(ERR_USERNOTINCHANNEL(client->getNickName(), nickName, channelNamee));
        return;
    }
    if (channel->get_channel_owner()->getNickName() == target->getNickName())
    {
        client->sendReply(ERR_NOKICKCHANNELOWNER(client->getNickName(), channelNamee));
        return;
    }
    if (client->isOperator())
    {
        channel->removeUserFromChannel(target);
        target->removeChannel(channel);
        string message = ":" + client->getPrefix() + " KICK " + channelNamee + " " + nickName + " :" + reason + "\n";
        channel->broadcastMessage(message, client);
        target->sendMessage(":" + client->getPrefix() + " KICK " + channelNamee + " " + nickName + " :" + reason);
        target->sendMessage("You have been kicked from channel " + channelNamee + " by " + client->getNickName());
        string leavemessage = client->getNickName() + " kicked " + target->getNickName() + " from channel " + channel->get_channel_name();
        log(leavemessage);
    }
    else
    {
        client->sendReply(ERR_CHANOPRIVSNEEDED(client->getNickName(), channelNamee));
        return;
    }
    if (channel->get_channel_client_count() == 0)
    {
        string channelName = channel->get_channel_name();
        srv->removeChannel(channelName);
    }
}
