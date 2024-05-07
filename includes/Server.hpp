#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h> // for socket()
#include <sys/types.h> // for socket()
#include <netinet/in.h> // for struct sockaddr_in
#include <fcntl.h> // for fcntl()
#include <unistd.h> // for close()
#include <arpa/inet.h> // for inet_ntoa()
#include "poll.h" // for poll()
#include "Client.hpp"
#include "Color.hpp"

class Client;
class Server {
private:
	int Port; //server port
	int ServerSocketFD; //server socket file descriptor
	static bool Signal; //static boolean to handle signals
	std::vector<Client> clients; //vector of clients
	std::vector<struct pollfd> fds; //vector of pollfd structures

public:
	Server();
	~Server();
	void ServerInit(); //initialize server
	void ServerSocket(); //create server socket
	void AcceptNewClient(); //accept new client
	void ReceiveData(int fd); //receive data from client

	static void SignalHandler(int signum); //signal handler

	void CloseFds(); //close file descriptor
	void ClearClients(int fd); //clear clients
};

#endif
