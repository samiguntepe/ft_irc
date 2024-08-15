#include "../../includes/Commands.hpp"

// PRIVMSG komutunu işleyen fonksiyon
void PrivMsg::privMsg(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 2)
    {
        client->sendReply(ERR_NEEDMOREPARAMS(client->getNickName(), "PRIVMSG"));
        return;
    }
    string commandString = mergeString(commandParts, " ");
    size_t targetStart = commandString.find("PRIVMSG") + 8;
    size_t messageStart = commandString.find(" :", targetStart);
    if (messageStart == string::npos)
    {
        client->sendReply(ERR_NOTEXTTOSEND(client->getNickName()));
        return;
    }
    string target = commandString.substr(targetStart, messageStart - targetStart);
    target = strim(target);
    string message = commandString.substr(messageStart + 2);
    if (target.at(0) == '#')
    {
        sendChannelMessage(client, target, message, srv);
    }
    else
    {
        sendPrivateMessage(client, target, message, srv);
    }
}

void sendChannelMessage(Client *client, string channel_name, string message, Server *srv)
{
    string bot_name = "reepNaoBot";
    Client *bot = srv->get_client(bot_name);
    Channel *channel = srv->getChannel(channel_name);
    if (channel == NULL)
    {
        client->sendReply(ERR_NOSUCHCHANNEL(client->getNickName(), channel_name));
        return;
    }
    if (channel->getModerated() && !client->isOperator())
    {
        client->sendReply(ERR_NOCANNOTSENDTOCHAN(client->getNickName(), channel_name));
        return;
    }
    if (channel->getNoExternalMessages() && !channel->isUserOnChannel(client))
    {
        client->sendReply(ERR_NOCANNOTSENDTOCHAN(client->getNickName(), channel_name));
        return;
    }
    channel->broadcastMessage(":" + client->getPrefix() + " PRIVMSG " + channel_name + " :" + message, client);
    bot->sendMessage(":" + client->getPrefix() + " PRIVMSG " + channel_name + " :" + message);
}

void sendPrivateMessage(Client *client, string target, string message, Server *srv)
{
    Client *target_client = srv->get_client(target);
    string bot_name = "reepNaoBot";
    Client *bot = srv->get_client(bot_name);
    if (target_client == NULL)
    {
        client->sendReply(ERR_NOSUCHNICK(client->getNickName(), target));
        return;
    }
    target_client->sendMessage(":" + client->getPrefix() + " PRIVMSG " + target + " :" + message);
    if (bot_name != target)
        bot->sendMessage(":" + client->getPrefix() + " PRIVMSG " + target + " :" + message);
}
