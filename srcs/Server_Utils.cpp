#include "../includes/Server.hpp"

void Server::process_partial_commands(int client_socket_fd)
{
	string& client_buffer = client_buffers[client_socket_fd].get_buffer();
	size_t endOfCommand;
	string command;
	if (client_buffer[0] == '/')
	{
		while ((endOfCommand = client_buffer.find('\n')) != string::npos)
		{
			command = client_buffer.substr(0, endOfCommand);
			client_buffer.erase(0, endOfCommand + 1);
			CommandParser::commandParser(command.c_str(), clients[client_socket_fd], this);
		}
		if (!client_buffer.find('\n'))
			client_buffer.clear();
	}
	else
	{
		while ((endOfCommand = client_buffer.find("\r\n")) != string::npos)
		{
			command = client_buffer.substr(0, endOfCommand);
			client_buffer.erase(0, endOfCommand + 2);
			CommandParser::commandParser(command.c_str(), clients[client_socket_fd], this);
		}
		client_buffer.clear();
	}
}

void Server::handle_client(int client_socket_fd)
{
	const size_t BUFFER_SIZE = 512;
	char tempBuffer[BUFFER_SIZE];
	memset(tempBuffer, 0, BUFFER_SIZE);
	ssize_t received = recv(client_socket_fd, tempBuffer, BUFFER_SIZE - 1, 0);
	if (received > 0)
	{
		client_buffers[client_socket_fd].append_to_buffer(string(tempBuffer, received));
		cout << "Received: " << tempBuffer << endl;
		process_partial_commands(client_socket_fd);
	} else if (received == 0 || errno == ECONNRESET)
	{
		FD_CLR(client_socket_fd, &read_set);
		clientDisconnect(client_socket_fd);
		client_buffers.erase(client_socket_fd);
	} else {
		if (errno != EAGAIN && errno != EWOULDBLOCK)
		{
			FD_CLR(client_socket_fd, &read_set);
			close(client_socket_fd);
			client_buffers.erase(client_socket_fd);
			throw RuntimeError("recv error.");
		}
	}
}

void Server::clientDisconnect(int client_socket_fd)
{
    try
    {
        std::map<int, Client*>::iterator it = clients.find(client_socket_fd);
        if (it == clients.end()) {
      	std::cout << "Client not found for removal." << std::endl;
            return;
        }
        remove_client_all_channels(it->second);
        it->second->leave();
        ostringstream message_streamDisconnect;
        message_streamDisconnect << "Client " << it->second->get_nick_name() << " has disconnected.";
        log(message_streamDisconnect.str());
        FD_CLR(client_socket_fd, &read_set);
        close(client_socket_fd);
        delete it->second;
        clients.erase(it);
    }
    catch (const std::exception &e)
    {
		std::cerr << e.what() << std::endl;
    }
}

void Server::setSrvPass(const string& pass) {
	password = pass;
}

bool Server::verify_server_password(const string& pass) {
	if (password == pass) {
		return true;
	}
	return false;
}

void Server::signal_handler(int signum)
{
	Server::get_instance()->shut_down_server();
	exit(signum);
}

void Server::shut_down_server()
{
	std::cout << "Shutting down the server..." << std::endl;
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		Client* client = it->second;
		if (client != NULL) {
			client->send_message("Shutting down the server. Your connection is being terminated.");
			remove_client_all_channels(client);
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
	std::cout <<"The server has been shut down." std::cout;
}
