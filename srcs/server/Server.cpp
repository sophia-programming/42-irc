#include "Server.hpp"

Server::Server() {
	ServerSocketFD = -1;
}

Server::~Server() {
}

void Server::makePoll(int socketFD) {
	struct pollfd NewPoll;

	NewPoll.fd = socketFD; // set pollfd file descriptor to socket
	NewPoll.events = POLLIN; // set pollfd events to POLLIN
	NewPoll.revents = 0; // set pollfd revents to 0
	fds.push_back(NewPoll); // add pollfd to vector

	if (socketFD != ServerSocketFD) { // check if the socket is not the server socket
		const std::string nick = "unknown" + std::to_string(socketFD); // set default nickname
		Client user(socketFD, nick); // create new user
		users[socketFD] = user; // add user to map
		std::string message = "Welcome to the chat room " + nickname[socketFD] + "\n";
		SendData(socketFD, message, message.size()); // send welcome message
	}
}

void Server::AcceptNewClient() {
	Client client; // create new client
	struct sockaddr_in clientAddress;
	socklen_t len = sizeof(clientAddress);

	int incomingFD = accept(ServerSocketFD, (struct sockaddr *)&clientAddress, &len); // accept new client
	if (incomingFD == -1)
		std::cout << RED << "accept() failed" << STOP << std::endl;

	if (fcntl(incomingFD, F_SETFL, O_NONBLOCK) == -1) // set client socket to non-blocking
		std::cout << RED << "fcntl() failed" << STOP << std::endl;

	client.setFD(incomingFD); // set client file descriptor
	client.SetIPAddress(inet_ntoa(clientAddress.sin_addr)); // set client IP address
	clients.push_back(client); // add client to vector
	makePoll(incomingFD); // call makePoll with the new client's FD

	std::cout << GREEN << "New client <" << incomingFD << "> connected" << STOP << std::endl;
}

void Server::ReceiveData(int fd) {
	char buff[1024]; // buffer to received data
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
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
	Client &user = users[fd]; // get user from map
	user.addMessage(std::string(buff)); // add message to user message buffer
	const std::string message = user.getMessage(); // get message from user message buffer

	//find CR LF (end point)
	if (message.find_first_of("\r\n"))
		user.parse();
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
		if (clients[i].getFD() == fd) {
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
		std::cout << RED << "Client " << clients[i].getFD() << " disconnected" << STOP << std::endl;
		close(clients[i].getFD());
	}
	if (ServerSocketFD != -1) { // close server socket
		std::cout << RED << "Server socket " << ServerSocketFD << " Disconnected" << STOP << std::endl;
		close(ServerSocketFD);
	}
}

void Server::ServerInit() {
	this->Port = 4444; // set port
	ServerSocket(); // create server socket

	std::cout << GREEN << "Server <" << ServerSocketFD << "> is listening on port " << Port << STOP << std::endl;

	while (Server::Signal == false) {
		if ((poll(&fds[0], fds.size(), -1) == -1) && Server::Signal == false) // wait for an event
			throw (std::runtime_error("poll() failed"));

		for (size_t i = 0; i < fds.size(); i++) { // check all file descriptors
			if (fds[i].revents & POLLIN) { // check if there is data to read
				if (fds[i].fd == ServerSocketFD)
					AcceptNewClient();
				else
					ReceiveData(fds[i].fd); // receive data from client
			}
		}
	}
	CloseFds();
}
