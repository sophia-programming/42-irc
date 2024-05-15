#include "Server.hpp"

Server::Server() {
	server_socketfd = -1;
}

Server::~Server() {
}

void Server::makePoll(int socketfd) {
	struct pollfd NewPoll;

	NewPoll.fd = socketfd; // set pollfd file descriptor to socket
	NewPoll.events = POLLIN; // set pollfd events to POLLIN
	NewPoll.revents = 0; // set pollfd revents to 0
	fds.push_back(NewPoll); // add pollfd to vector

	if (socketfd != server_socketfd) { // check if the socket is not the server socket
		const std::string nick = "unknown" + std::to_string(socketfd); // set default nickname
		Client user(socketfd, nick); // create new user
		users[socketfd] = user; // add user to map
		std::string message = "Welcome to the chat room " + nickname[socketfd] + "\n";
		SendData(socketfd, message, message.size()); // send welcome message
	}
}

void Server::AcceptNewClient() {
	Client client; // create new client
	struct sockaddr_in clientAddress;
	socklen_t len = sizeof(clientAddress);

	int incomingfd = accept(server_socketfd, (struct sockaddr *)&clientAddress, &len); // accept new client
	if (incomingfd == -1)
		std::cout << RED << "accept() failed" << STOP << std::endl;

	if (fcntl(incomingfd, F_SETFL, O_NONBLOCK) == -1) // set client socket to non-blocking
		std::cout << RED << "fcntl() failed" << STOP << std::endl;

	client.setfd(incomingfd); // set client file descriptor
	client.SetIPAddress(inet_ntoa(clientAddress.sin_addr)); // set client IP address
	clients.push_back(client); // add client to vector
	makePoll(incomingfd); // call makePoll with the new client's fd

	std::cout << GREEN << "New client <" << incomingfd << "> connected" << STOP << std::endl;
}

void Server::ReceiveData(int fd) {
	char buff[1024] = {0}; // buffer to received data
	memset(buff, 0, sizeof(buff)); // clear buffer

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); // receive data from client

	if (bytes <= 0) {
		std::cout << RED << "Client " << fd << " disconnected" << STOP << std::endl;
		ClearClients(fd);
		close(fd);
	}
	else {
		buff[bytes] = '\0';
		std::cout << YELLOW << "Client <" << fd << "> : " << buff << STOP;
	}
	Client &user = users[fd]; // get user from map
	user.addMessage(std::string(buff)); // add message to user message buffer
	const std::string message = user.getMessage(); // get message from user message buffer

	//find CR LF (end point)
	if (message.find("\r\n"))
		user.parse(message); // parse message
}

void Server::SendData(int fd, std::string message, int size) {
	send(fd, message.c_str(), size, 0); // send data to client
}

void Server::ClearClients(int fd) {
	for (size_t i = 0; i < fds.size(); i++) { // clear from the pollfd
		if (fds[i].fd == fd) {
			fds.erase(fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < clients.size(); i++) { // clear clients from  the vector
		if (clients[i].getfd() == fd) {
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
	address.sin_port = htons(this->port); // set port

	server_socketfd = socket(AF_INET, SOCK_STREAM, 0); // create server socket
	if (server_socketfd == -1)  // check if socket creation failed
		throw (std::runtime_error("Server socket creation failed"));

	int en = 1;
	if (setsockopt(server_socketfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1) // set socket options to reuse address
		throw (std::runtime_error("Server socket options failed"));
	if (fcntl(server_socketfd, F_SETFL, O_NONBLOCK) == -1) // set socket to non-blocking
		throw (std::runtime_error("Server socket fcntl failed"));
	if (bind(server_socketfd, (struct sockaddr *)&address, sizeof(address)) == -1) // bind server socket to address
		throw (std::runtime_error("Server socket bind failed"));
	if (listen(server_socketfd, SOMAXCONN) == -1) // listen on server socket
		throw(std::runtime_error("Server socket listen failed"));

	NewPoll.fd = server_socketfd; // set pollfd file descriptor to server socket
	NewPoll.events = POLLIN; // set pollfd events to POLLIN
	NewPoll.revents = 0; // set pollfd revents to 0
	fds.push_back(NewPoll); // add pollfd to vector
}

void Server::CloseFds() {
	for (size_t i = 0; i < clients.size(); i++) { // close all clients
		std::cout << RED << "Client " << clients[i].getfd() << " disconnected" << STOP << std::endl;
		close(clients[i].getfd());
	}
	if (server_socketfd != -1) { // close server socket
		std::cout << RED << "Server socket " << server_socketfd << " Disconnected" << STOP << std::endl;
		close(server_socketfd);
	}
}

void Server::ServerInit() {
	this->port = 4444; // set port
	ServerSocket(); // create server socket

	std::cout << GREEN << "Server <" << server_socketfd << "> is listening on port " << port << STOP << std::endl;

	while (Server::signal == false) {
		if ((poll(&fds[0], fds.size(), -1) == -1) && Server::signal == false) // wait for an event
			throw (std::runtime_error("poll() failed"));

		for (size_t i = 0; i < fds.size(); i++) { // check all file descriptors
			if (fds[i].revents & POLLIN) { // check if there is data to read
				if (fds[i].fd == server_socketfd)
					AcceptNewClient();
				else
					ReceiveData(fds[i].fd); // receive data from client
			}
		}
	}
	CloseFds();
}
