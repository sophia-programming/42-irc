#include "Server.hpp"

Server::Server(char *port, char *password) {
	std::cout << "Server started at port " << port
		<< " with password: " << password << std::endl;
}

Server::~Server() {
}

void Server::MakePoll(int socketfd) {
	struct pollfd NewPoll;

	NewPoll.fd = socketfd; // set pollfd file descriptor to socket
	NewPoll.events = POLLIN; // set pollfd events to POLLIN
	NewPoll.revents = 0; // set pollfd revents to 0
	fds_.push_back(NewPoll); // add pollfd to vector

	if (socketfd != server_socket_fd_) { // check if the socket is not the server socket
		const std::string nick = "unknown" + std::to_string(socketfd); // set default nickname
		Client user(socketfd, nick); // create new user
		users_[socketfd] = user; // add user to map
		std::string message = "Welcome to the chat room " + nickname_[socketfd] + "\n";
		SendData(socketfd, message, message.size()); // send welcome message
	}
}

void Server::AcceptNewClient() {
	Client client; // create new client
	struct sockaddr_in clientAddress;
	socklen_t len = sizeof(clientAddress);

	int incomingfd = accept(server_socket_fd_, (struct sockaddr *)&clientAddress, &len); // accept new client
	if (incomingfd == -1)
		std::cout << RED << "accept() failed" << STOP << std::endl;

	if (fcntl(incomingfd, F_SETFL, O_NONBLOCK) == -1) // set client socket to non-blocking
		std::cout << RED << "fcntl() failed" << STOP << std::endl;

	client.SetFd(incomingfd); // set client file descriptor
	client.SetIPAddress(inet_ntoa(clientAddress.sin_addr)); // set client IP address
	connected_clients.push_back(client); // add client to vector
	MakePoll(incomingfd); // call MakePoll with the new client's fd

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
	Client &user = users_[fd]; // get user from map
	user.AddMessage(std::string(buff)); // add message to user message buffer
	const std::string message = user.GetMessage(); // get message from user message buffer

	//find CR LF (end point)
	if (message.find("\r\n"))
		user.Parse(message); // parse message
}

void Server::SendData(int fd, std::string message, int size) {
	send(fd, message.c_str(), size, 0); // send data to client
}

void Server::ClearClients(int fd) {
	for (size_t i = 0; i < fds_.size(); i++) { // clear from the pollfd
		if (fds_[i].fd == fd) {
			fds_.erase(fds_.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < connected_clients.size(); i++) { // clear clients from  the vector
		if (connected_clients[i].GetFd() == fd) {
			connected_clients.erase(connected_clients.begin() + i);
			break;
		}
	}
}

void Server::ServerSocket() {
	struct sockaddr_in address; // server address
	struct pollfd NewPoll; // pollfd structure
	address.sin_family = AF_INET; // set address family to IPv4
	address.sin_addr.s_addr = INADDR_ANY; // set address to any interface
	address.sin_port = htons(this->port_); // set port

	server_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0); // create server socket
	if (server_socket_fd_ == -1)  // check if socket creation failed
		throw (std::runtime_error("Server socket creation failed"));

	int en = 1;
	if (setsockopt(server_socket_fd_, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1) // set socket options to reuse address
		throw (std::runtime_error("Server socket options failed"));
	if (fcntl(server_socket_fd_, F_SETFL, O_NONBLOCK) == -1) // set socket to non-blocking
		throw (std::runtime_error("Server socket fcntl failed"));
	if (bind(server_socket_fd_, (struct sockaddr *)&address, sizeof(address)) == -1) // bind server socket to address
		throw (std::runtime_error("Server socket bind failed"));
	if (listen(server_socket_fd_, SOMAXCONN) == -1) // listen on server socket
		throw(std::runtime_error("Server socket listen failed"));

	NewPoll.fd = server_socket_fd_; // set pollfd file descriptor to server socket
	NewPoll.events = POLLIN; // set pollfd events to POLLIN
	NewPoll.revents = 0; // set pollfd revents to 0
	fds_.push_back(NewPoll); // add pollfd to vector
}

void Server::CloseFds() {
	for (size_t i = 0; i < connected_clients.size(); i++) { // close all clients
		std::cout << RED << "Client " << connected_clients[i].GetFd() << " disconnected" << STOP << std::endl;
		close(connected_clients[i].GetFd());
	}
	if (server_socket_fd_ != -1) { // close server socket
		std::cout << RED << "Server socket " << server_socket_fd_ << " Disconnected" << STOP << std::endl;
		close(server_socket_fd_);
	}
}

void Server::ServerInit() {
	this->port_ = 6667; // set port
	ServerSocket(); // create server socket

	std::cout << GREEN << "Server <" << server_socket_fd_ << "> is listening on port " << port_ << STOP << std::endl;

	while (Server::signal_ == false) {
		if ((poll(&fds_[0], fds_.size(), -1) == -1) && Server::signal_ == false) // wait for an event
			throw (std::runtime_error("poll() failed"));

		for (size_t i = 0; i < fds_.size(); i++) { // check all file descriptors
			if (fds_[i].revents & POLLIN) { // check if there is data to read
				if (fds_[i].fd == server_socket_fd_)
					AcceptNewClient();
				else
					ReceiveData(fds_[i].fd); // receive data from client
			}
		}
	}
	CloseFds();
}


// 既存のチャンネルか確認する
// 1: std::string& name -> 確認したいチャンネル名
//bool Server::IsChannel(std::string& name) {
//	std::map<std::string, Channel>::iterator iter = this->channel_list_.find(name);
//	if(iter != this->channel_list_.end()){
//		return true;
//	}
//	return false;
//}

// チャンネル名から検索してchannelオブジェクトを取得する
// 1:std::string& name -> 取得したいチャンネル名
//Channel Server::GetChannel(std::string& name)
//{
//	std::map<std::string, Channel>::iterator iter = this->channel_list_.find(name);
//	if(iter != this->channel_list_.end()){
//		return iter->second;
//	}
//	throw std::exception();
//}

//チャンネルを作成してリストに登録する
// 1:std::string& name　-> 作成したいチャンネル名
//void Server::CreateChannel(std::string& name)
//{
//	if(name[0] != '#'){
//		// error plese create #channel name
//		return;
//	}
//	this->channel_list_.emplace(name, Channel(name));
//}
