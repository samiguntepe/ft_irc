#include "../includes/Server.hpp"

int Server::is_nick_exist(std::string const& nick)
{
    for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->_nick == nick)
            return 1;
    }
    return 0;
}

void Server::kick_client(cliIt& it)
{
    std::cout << RED << "Client " << it->_cliFd - 3  << " disconnected!" << RESET << std::endl;
    FD_CLR(it->_cliFd, &_readFds);
    FD_CLR(it->_cliFd, &_writeFds);
    close(it->_cliFd);
    _clients.erase(it);
}

void Server::pass_checker(Client& client)
{
    if (client._passChecked == 0)
    {
        std::cout << RED << client._cliFd << " is not logged in." << RESET << std::endl;
        for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
            if (client._cliFd == it->_cliFd)
            {
                Utils::write_message(client._cliFd, ERR_PASSWDMISMATCH);
                kick_client(it);
                break;
            }
        }
    }
}

void Server::get_after_colon(std::vector<std::string>& params)
{
    params[1].erase(0, 1);
    for (size_t i = 2; i < params.size(); ++i)
    {
        params[1] += " " + params[i];
    }
}

Channel& Server::get_channel(std::string const& channelName)
{
    chanIt it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->_name == channelName) {
            break;
        }
    }
    return *it;
}

int Server::is_channel_exist(std::string const& channelName)
{
    for (chanIt it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->_name == channelName)
            return 1;
    }
    return 0;
}

int Server::client_is_in_there(Client& client, std::string const& chanName)
{
    for (chanIt it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->_name == chanName)
        {
            for (cliIt it2 = it->_channelClients.begin(); it2 != it->_channelClients.end(); ++it2)
            {
                if (it2->_nick == client._nick)
                    return (1);
            }
        }
    }
    return (0);
}

int Server::get_op_fd(std::string const& opName)
{
    for (cliIt it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->_nick == opName)
            return (it->_cliFd);
    }
    return (0);
}

void Server::show_right_gui(Client &cli, Channel &tmp)
{
    std::string msg;
    if (tmp._name.empty())
        return;
    for(std::vector<Client>::iterator it = tmp._channelClients.begin() ; it != tmp._channelClients.end(); ++it)
    {
        if (it->_cliFd == get_op_fd(tmp._opNick))
            msg += "@";
        msg += (*it)._nick + " ";
    }
    Utils::write_all_message(tmp.getFds(), RPL_NAMREPLY(cli._nick, tmp._name, msg));
}

void Server::set_port(size_t const& port)
{
    _port = port;
}

void Server::set_password(std::string const& password)
{
    _password = password;
}

void Server::print_status()
{
    std::cout << CYAN << "Server running on this pc " << RESET << std::endl;
    std::cout << CYAN << "Password: " << _password << RESET << std::endl;
    std::cout << CYAN << "Port: " << _port << RESET << std::endl;
}
