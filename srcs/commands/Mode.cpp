#include "../../includes/Commands.hpp"

void Mode::mode(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 2)
    {
        client->send_message(":" + client->get_host_name() + " 461 " + client->get_nick_name() + " MODE :Not enough parameters");
        return;
    }
    string atargetE = commandParts.at(1);
    string mode = (commandParts.size() > 2) ? commandParts.at(2) : "";
    string modeParams = "100";
    if (atargetE.empty())
    {
        client->send_message(":" + client->get_host_name() + " 403 " + client->get_nick_name() + " " + atargetE + " :No such channel\r\n");
        return;
    }
    else if (atargetE.at(0) == '#')
    {
        string atarget = atrim(atargetE);
        Channel *channel = srv->get_channel(atarget);
        if (!channel)
        {
            client->send_message(":" + client->get_host_name() + " 403 " + client->get_nick_name() + " " + atargetE + " :No such channel\r\n");
            return;
        }
        if (mode.empty())
            return;
        if (mode.at(0) == '+')
        {
            if (commandParts.size() > 3)
            {
                modeParams = commandParts.at(3);
                channel->set_up_mode_channel(channel, client, mode, modeParams);
            }
            else
                channel->set_up_mode_channel(channel, client, mode, modeParams);
        }
        else if (mode.at(0) == '-')
        {
            if (commandParts.size() > 3)
            {
                modeParams = commandParts.at(3);
                channel->set_low_mode_channel(channel, client, mode, modeParams);
            }
            else
                channel->set_low_mode_channel(channel, client, mode, modeParams);
        }
        else
            client->send_message(":" + client->get_host_name() + " 501 " + client->get_nick_name() + " :Invalid MODE flag");
    }
    else
        client->send_message(":" + client->get_host_name() + " 501 " + client->get_nick_name() + " :Invalid MODE flag");
}
