#include "../../includes/Server.hpp"

int Server::check_nick_depends(std::vector<std::string>& params, Client& cli)
{
    if (cli._isCap == NC) {
        pass_checker(cli);
    }
    if (!(params.size() == 1 && !is_nick_exist(params[0]) && !params[0].empty() && params[0].size() <= 9))
    {
        if (params.size() != 1)
            Utils::write_message(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        else if (params[0].size() > 9)
            Utils::write_message(cli._cliFd, ERR_NONICKNAMEGIVEN);
        else if (params[0].empty())
            Utils::write_message(cli._cliFd, ERR_NICKNAMEEMPTY(cli._nick));
        else
            Utils::write_message(cli._cliFd, ERR_NICKNAMEINUSE(params[0]));
        return (0);
    }
    return (1);
}

void Server::Nick(std::vector<std::string>& params, Client& cli)
{
    if (!check_nick_depends(params, cli))
        return ;
    std::string oldNick = cli._nick;
    cli._nick = params[0];

    for (chanIt it = _channels.begin(); it != _channels.end(); ++it) {
        for (cliIt cit = it->_channelClients.begin(); cit != it->_channelClients.end(); ++cit) {
            if (oldNick == cit->_nick)
            {
                if (oldNick == it->_opNick)
                    it->_opNick = params[0];
                cit->_nick = params[0];
                break;
            }
        }
        Utils::write_message(cli._cliFd, RPL_NICK(oldNick, cli._user, cli._ipAddr, params[0]));
        show_right_gui(cli, *it);
    }
    Utils::write_message(cli._cliFd, RPL_NICK(oldNick, cli._user, cli._ipAddr, params[0]));
}
