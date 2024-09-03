#include "../../includes/Server.hpp"

void Server::Who(std::vector<std::string>& params, Client& cli)
{
    pass_checker(cli);
    if (params.size() > 1)
        return ;
    if (is_channel_exist(params[0]) == false)
        Utils::write_message(cli._cliFd,"No such channel\r\n");
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->_name == params[0])
        {
            Utils::write_message(cli._cliFd, "Users in this channel:\r\n");
            for (cliIt it2 = it->_channelClients.begin(); it2 != it->_channelClients.end(); ++it2) {
                Utils::write_message(cli._cliFd, "* " + it2->_nick + "\r\n");
            }
            return;
        }
    }
}
