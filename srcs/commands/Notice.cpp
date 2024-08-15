#include "../../includes/Commands.hpp"

static void sendNoticeChannelMessage(Client *client, string channel_name, string message, Server *srv)
{
    Channel *channel = srv->getChannel(channel_name);
    if (channel == NULL)
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channel_name));
        return;
    }
    channel->broadcastMessage(":" + client->getPrefix() + " NOTICE " + channel_name + " :" + message);
}

static void sendNoticeMessage(Client *client, string target, string message, Server *srv)
{
    Client *target_client = srv->get_client(target);
    if (target_client == NULL)
    {
        client->sendReply(ERR_NOSUCHNICK(client->getNickName(), target));
        return;
    }
    target_client->sendMessage(":" + client->getPrefix() + " NOTICE " + target + " :" + message);
}

void Notice::notice(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 3)
    {
        client->sendMessage(":" + client->getHostName() + " 461 " + client->getNickName() + " NOTICE : Not enough parameters");
        return;
    }
    string commandString = mergeString(commandParts, " ");
    size_t targetStart = commandString.find("NOTICE") + 7;
    size_t messageStart = commandString.find(" :", targetStart);
    if (messageStart == string::npos)
    {
        client->sendMessage(":" + client->getHostName() + " 412 " + client->getNickName() + "  NOTICE :No text to send");
        return;
    }
    string target = commandString.substr(targetStart, messageStart - targetStart);
    string message = commandString.substr(messageStart + 2);
    if (target.empty())
    {
        client->sendMessage(":" + client->getHostName() + " 401 " + client->getNickName() + " NOTICE :No such nick/channel");
        return;
    }
    if (message.empty())
    {
        client->sendMessage(":" + client->getHostName() + " 412 " + client->getNickName() + " NOTICE :No text to send");
        return;
    }
    target = strim(target);
    if (target.at(0) == '#')
    {
        sendNoticeChannelMessage(client, target, message, srv);
    }
    else
    {
        sendNoticeMessage(client, target, message, srv);
    }
}
