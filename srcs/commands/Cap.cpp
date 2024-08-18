#include "../../includes/Commands.hpp"

void Cap::cap(Client *client, const vector<string> commandParts)
{
    if (commandParts.size() < 2)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "CAP"));
        return;
    }
    string cap = commandParts.at(1);
    if (cap == "LS" || cap == "ls" )
    {
        client->send_message("/PASS\n/USER\n/NICK\n/JOIN\n/PART\n/PRIVMSG\n/NOTICE\n/KICK\n/MODE\n/QUIT\n");
    }
    else
    {
        client->send_reply(ERR_INVALIDCAPCMD(client->get_nick_name()));
    }
}
