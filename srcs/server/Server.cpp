#include "Server.hpp"

Server::Server() {
	ServerSocketFD = -1;
	std::cout << "Server default constructor" << std::endl;
}

Server::~Server() {
}

void Server::ClearClients(int fd) {
	for (size_t i = 0; i < fds.size(); i++) { // clear from the pollfd
		if (fds[i].fd == fd) {
			fds.erase(fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < clients.size(); i++) { // clear clients from  the vector
		if (clients[i].GetFD() == fd) {
			clients.erase(clients.begin() + i);
			break;
		}
	}
}
