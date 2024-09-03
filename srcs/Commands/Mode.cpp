#include "../../includes/Server.hpp"

void Server::Mode(std::vector<std::string>& params, Client& cli)
{
    pass_checker(cli);
    if (params.size() == 1)
        return;
    if (params.size() < 1 || params.size() > 3) {
        Utils::write_message(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        return ;
    }
    if (!is_channel_exist(params[0])) {
        Utils::write_message(cli._cliFd, ERR_NOSUCHCHANNEL(cli._nick, params[0]));
        return;
    }
    for (chanIt it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->_name == params[0])
        {
            if (it->_opNick != cli._nick) {
                Utils::write_message(cli._cliFd, ERR_CHANOPRIVSNEEDED(cli._nick, params[0]));
                return ;
            }
            int flag = 0;
            modes_op(it, params, &flag);
            modes_limit(it, params, &flag);
            modes_key(it, params, &flag);
            if (!flag) {
                Utils::write_message(cli._cliFd, ERR_UNKNOWNMODE(cli._nick, params[0], params[1]));
                return ;
            }
        }
    }
}

void Server::modes_op(chanIt& it, std::vector<std::string>& params, int* flag)
{
    if (params[1] == "+o")
    {
        *flag = 1;
        size_t flag2 = 0;
        for (cliIt it2 = it->_channelClients.begin(); it2 != it->_channelClients.end(); ++it2) {
            if (it2->_nick == params[2])
            {
                if (it2->_nick == it->_opNick)
                    return ;
                flag2 = 1;
                size_t i;
                for (i = 0; i < it->_channelClients.size(); ++i) {
                    if (it->_channelClients[i]._nick == it2->_nick)
                        break ;
                }
                Client tmp = it->_channelClients[i];
                it->_channelClients[i] = it->_channelClients[0];
                it->_channelClients[0] = tmp;
                it->_opNick = it->_channelClients[0]._nick;
                show_right_gui(tmp, *it);
                return ;
            }
        }
        if (flag2 == 0) 
        {
            Utils::write_message(get_op_fd(it->_opNick), "User is not in the channel\r\n");
            return ;
        }
    }
}

void Server::modes_limit(chanIt& it, std::vector<std::string>& params, int* flag)
{
    if (params[1] != "+l")
        return ;
    *flag = 1;
    if (params[1] == "+l" && params.size() == 3)
        it->_userLimit = std::atoi(params[2].c_str());
    else if (params[1] == "+l" && params.size() != 3)
        std::cout << "Not enough paramaters for '+l' " << std::endl;
    Utils::write_message(get_op_fd(it->_opNick), RPL_MODE(it->_opNick, params[0], "+l", params[2]));
    return ;
}

void Server::modes_key(chanIt& it, std::vector<std::string>& params, int* flag)
{
    if (params[1] != "+k" && params[1] != "-k")
        return ;
    if (params[1] == "+k" && params.size() != 3)
        return ;
    if(params[1] == "+k")
        it->_key = params[2];
    else if (params[1] == "-k")
        it->_key = "";
    *flag = 1;
}
