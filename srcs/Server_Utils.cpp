#include "../includes/Server.hpp"

void Server::process_partial_commands(int clientSocketFD)
{
	string& clientBuffer = clientBuffers[clientSocketFD].get_buffer();
	size_t endOfCommand;
	string command;
	if (clientBuffer[0] == '/')
	{
		while ((endOfCommand = clientBuffer.find('\n')) != string::npos)
		{
			command = clientBuffer.substr(0, endOfCommand);
			clientBuffer.erase(0, endOfCommand + 1);
			CommandParser::commandParser(command.c_str(), _clients[clientSocketFD], this);
		}
		if (!clientBuffer.find('\n'))
			clientBuffer.clear();
	}
	else
	{
		while ((endOfCommand = clientBuffer.find("\r\n")) != string::npos)
		{
			command = clientBuffer.substr(0, endOfCommand);
			clientBuffer.erase(0, endOfCommand + 2);
			CommandParser::commandParser(command.c_str(), _clients[clientSocketFD], this);
		}
		clientBuffer.clear();
	}
}

void Server::handle_client(int clientSocketFD)
{
	const size_t BUFFER_SIZE = 512;
	char tempBuffer[BUFFER_SIZE];
	memset(tempBuffer, 0, BUFFER_SIZE);
	ssize_t received = recv(clientSocketFD, tempBuffer, BUFFER_SIZE - 1, 0);
	if (received > 0) {
		clientBuffers[clientSocketFD].append_to_buffer(string(tempBuffer, received));
		cout << "Received: " << tempBuffer << endl;
		process_partial_commands(clientSocketFD);
	} else if (received == 0 || errno == ECONNRESET) {
		FD_CLR(clientSocketFD, &read_set);
		client_disconnect(clientSocketFD);
		clientBuffers.erase(clientSocketFD);
	} else {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			FD_CLR(clientSocketFD, &read_set);
			ErrorLogger("recv error", __FILE__, __LINE__);
			close(clientSocketFD);
			clientBuffers.erase(clientSocketFD);
		}
	}
}

void Server::client_disconnect(int clientSocketFD)
{
    try
    {
        std::map<int, Client*>::iterator it = _clients.find(clientSocketFD);
        if (it == _clients.end()) {
            write(STDOUT_FILENO, "Client not found for removal.\n", 30);
            return;
        }
        remove_client_from_all_channels(it->second);
        it->second->leave();
        FD_CLR(clientSocketFD, &read_set);
        close(clientSocketFD);
        delete it->second;
        _clients.erase(it);
    }
    catch (const std::exception &e)
    {
        write(STDOUT_FILENO, e.what(), strlen(e.what()));
    }
}

void Server::set_server_password(const string& pass) {
	_serverPass = pass;
}

bool Server::verify_server_password(const string& pass) {
	if (_serverPass == pass) {
		return true;
	}
	return false;
}

void Server::signal_handler(int signum)
{
	Server::get_instance()->shutdown_server();
	exit(signum);
}

void Server::shutdown_server()
{
	string outmessage = "Shutting down the server...\n";
	write(STDOUT_FILENO, outmessage.c_str(), outmessage.size());
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		Client* client = it->second;
		if (client != NULL) {
			client->send_message("Shutting down the server. Your connection is being terminated.");
			remove_client_from_all_channels(client);
			close(it->first);
			delete client;
		}
	}
	_clients.clear();
	if (_serverSocketFD != -1) {
		close(_serverSocketFD);
		_serverSocketFD = -1;
	}
	if (_bot != NULL) {
		delete _bot;
		_bot = NULL;
	}
	FD_ZERO(&read_set);
	string outmessage2 = "The server has been shut down.\n";
	write(STDOUT_FILENO, outmessage2.c_str(), outmessage2.size());
}
