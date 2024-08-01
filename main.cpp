
#include "server.hpp"

int main(int argc, char **argv)
{
	 try {
        Server server;
        server.arg_control(argv);
    } catch (const RuntimeError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
	


	// sockaddr_in client_addr;
	// socklen_t client_len = sizeof(client_addr);
	// char buff[1024];
	// fd_set read_fds;
	// std::vector<int> client_fds;

	// while (true)
	// {
	// 	int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
	// 	if (client_fd < 0)
	// 	{
	// 		std::cerr << "Accept failed." << std::endl;
	// 		close(socket_fd);
	// 		return 1;
	// 	}

	// 	std::cout << "Client number " << client_fd << " connected." << std::endl;
	// 	client_fds.push_back(client_fd);

	// 	while (true)
	// 	{
	// 		FD_ZERO(&read_fds);
	// 		FD_SET(socket_fd, &read_fds);
	// 		int max_fd = socket_fd;

	// 		for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end(); ++it)
	// 		{
	// 			int fd = *it;
	// 			FD_SET(fd, &read_fds);
	// 			if (fd > max_fd)
	// 			{
	// 				max_fd = fd;
	// 			}
	// 		}

	// 		int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);

	// 		if (activity < 0)
	// 		{
	// 			std::cerr << "Select error." << std::endl;
	// 			break;
	// 		}

	// 		if (FD_ISSET(socket_fd, &read_fds))
	// 		{
	// 			int new_client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
	// 			if (new_client_fd < 0)
	// 			{
	// 				std::cerr << "Accept failed." << std::endl;
	// 				continue;
	// 			}
	// 			std::cout << "\nClient number " << new_client_fd << " connected.\n" << std::endl;
	// 			client_fds.push_back(new_client_fd);
	// 		}

	// 		for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end();)
	// 		{
	// 			int fd = *it;
	// 			if (FD_ISSET(fd, &read_fds))
	// 			{
	// 				ssize_t bytes_received = recv(fd, buff, sizeof(buff), 0);
	// 				if (bytes_received > 0)
	// 				{
	// 					buff[bytes_received] = '\0';
	// 					std::cout << "Client number " << *it << " message: " << buff << std::endl;
	// 				}
	// 				else
	// 				{
	// 					if (bytes_received == 0)
	// 					{
	// 						std::cout << "\nClient number " << *it << " disconnected\n" << std::endl;
	// 					}
	// 					else
	// 					{
	// 						std::cerr << "Receive failed." << std::endl;
	// 					}
	// 					close(fd);
	// 					it = client_fds.erase(it);
	// 					continue;
	// 				}
	// 			}
	// 			++it;
	// 		}
	// 	}
	// }
	// close(socket_fd);
	// return 0;
}
