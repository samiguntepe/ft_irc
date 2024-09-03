#include "../../includes/Server.hpp"

void Server::Invite(std::vector<std::string>& params, Client& cli)
{
    pass_checker(cli);
    if (is_channel_exist(params[1]) == 0) {
        Utils::write_message(cli._cliFd, ERR_NOSUCHCHANNEL(params[1], params[0]));
        return ;
    }
    size_t flag = 0;
    for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->_nick == params[0])
        {
            flag = 1;
            Channel chan = get_channel(params[1]);
            if (cli._nick != chan._opNick) {
                Utils::write_message(cli._cliFd, ERR_CHANOPRIVSNEEDED(cli._nick, params[1]));
                return ;
            }
            Utils::write_message(it->_cliFd, RPL_INVITE(cli._nick, cli._ipAddr, chan._name, params[1]));
        }
    }
    if (flag == 0)
        Utils::write_message(cli._cliFd, ERR_NOSUCHNICK);
}
