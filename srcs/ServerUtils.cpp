#include "../includes/Server.hpp"

void Server::processPartialCommands(int client_socket_fd)
{
	string& clientBuffer = clientBuffers[client_socket_fd].getBuffer();
	size_t endOfCommand;
	string command;
	if (clientBuffer[0] == '/')
	{
		while ((endOfCommand = clientBuffer.find('\n')) != string::npos)
		{
			command = clientBuffer.substr(0, endOfCommand);
			clientBuffer.erase(0, endOfCommand + 1);
			CommandParser::commandParser(command.c_str(), clients[client_socket_fd], this);
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
			CommandParser::commandParser(command.c_str(), clients[client_socket_fd], this);
		}
		clientBuffer.clear();
	}
}

void Server::handleClient(int client_socket_fd)
{
	const size_t BUFFER_SIZE = 512;
	char tempBuffer[BUFFER_SIZE];
	memset(tempBuffer, 0, BUFFER_SIZE);
	ssize_t received = recv(client_socket_fd, tempBuffer, BUFFER_SIZE - 1, 0);
	if (received > 0) {
		clientBuffers[client_socket_fd].appendtoBuffer(string(tempBuffer, received));
		cout << "Received: " << tempBuffer << endl;
		processPartialCommands(client_socket_fd);
	} else if (received == 0 || errno == ECONNRESET) {
		FD_CLR(client_socket_fd, &read_set);
		clientDisconnect(client_socket_fd);
		clientBuffers.erase(client_socket_fd);
	} else {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			FD_CLR(client_socket_fd, &read_set);
			ErrorLogger("recv error", __FILE__, __LINE__);
			close(client_socket_fd);
			clientBuffers.erase(client_socket_fd);
		}
	}
}

void Server::clientDisconnect(int client_socket_fd)
{
    try
    {
        std::map<int, Client*>::iterator it = clients.find(client_socket_fd);
        if (it == clients.end()) {
            write(STDOUT_FILENO, "Client not found for removal.\n", 30);
            return;
        }
        removeClientFromAllChannels(it->second);
        it->second->leave();
        ostringstream message_streamDisconnect;
        message_streamDisconnect << "Client " << it->second->getNickName() << " has disconnected.";
        log(message_streamDisconnect.str());
        FD_CLR(client_socket_fd, &read_set);
        close(client_socket_fd);
        delete it->second;
        clients.erase(it);
    }
    catch (const std::exception &e)
    {
        write(STDOUT_FILENO, e.what(), strlen(e.what()));
    }
}

void Server::setSrvPass(const string& pass) {
	password = pass;
}

bool Server::verifySrvPass(const string& pass) {
	if (password == pass) {
		return true;
	}
	return false;
}

void Server::signalHandler(int signum)
{
	Server::getInstance()->shutdownSrv();
	exit(signum);
}

void Server::shutdownSrv()
{
	string outmessage = "Shutting down the server...\n";
	write(STDOUT_FILENO, outmessage.c_str(), outmessage.size());
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		Client* client = it->second;
		if (client != NULL) {
			client->sendMessage("Shutting down the server. Your connection is being terminated.");
			removeClientFromAllChannels(client);
			close(it->first);
			delete client;
		}
	}
	clients.clear();
	if (server_fd != -1) {
		close(server_fd);
		server_fd = -1;
	}
	if (_bot != NULL) {
		delete _bot;
		_bot = NULL;
	}
	FD_ZERO(&read_set);
	string outmessage2 = "The server has been shut down.\n";
	write(STDOUT_FILENO, outmessage2.c_str(), outmessage2.size());
}
