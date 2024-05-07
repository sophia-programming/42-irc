#include "Server.hpp"

Server::Server() {
	ServerSocketFD = -1;
}

Server::~Server() {
}

void Server::ServerInit() {
	this->Port = 4444; // set port
	ServerSocket(); // create server socket

	std::cout << GREEN << "Server <" << ServerSocketFD << "> is listening on port " << Port << STOP << std::endl;
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

void Server::ServerSocket() {
	struct sockaddr_in address; // server address
	struct pollfd NewPoll; // pollfd structure
	address.sin_family = AF_INET; // set address family to IPv4
	address.sin_addr.s_addr = INADDR_ANY; // set address to any interface
	address.sin_port = htons(this->Port); // set port

	ServerSocketFD = socket(AF_INET, SOCK_STREAM, 0); // create server socket
	if (ServerSocketFD == -1)  // check if socket creation failed
		throw (std::runtime_error("Server socket creation failed"));

	int en = 1;
	if (setsockopt(ServerSocketFD, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1) // set socket options to reuse address
		throw (std::runtime_error("Server socket options failed"));
	if (fcntl(ServerSocketFD, F_SETFL, O_NONBLOCK) == -1) // set socket to non-blocking
		throw (std::runtime_error("Server socket fcntl failed"));
	if (bind(ServerSocketFD, (struct sockaddr *)&address, sizeof(address)) == -1) // bind server socket to address
		throw (std::runtime_error("Server socket bind failed"));
	if (listen(ServerSocketFD, SOMAXCONN) == -1) // listen on server socket
		throw(std::runtime_error("Server socket listen failed"));

	NewPoll.fd = ServerSocketFD; // set pollfd file descriptor to server socket
	NewPoll.events = POLLIN; // set pollfd events to POLLIN
	NewPoll.revents = 0; // set pollfd revents to 0
	fds.push_back(NewPoll); // add pollfd to vector
}

void Server::CloseFds() {
	for (size_t i = 0; i < clients.size(); i++) { // close all clients
		std::cout << RED << "Client " << clients[i].GetFD() << " disconnected" << STOP << std::endl;
		close(clients[i].GetFD());
	}
	if (ServerSocketFD != -1) { // close server socket
		std::cout << RED << "Server socket " << ServerSocketFD << " Disconnected" << STOP << std::endl;
		close(ServerSocketFD);
	}
}
