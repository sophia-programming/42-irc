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
#include <map> // for std::map
#include "poll.h" // for poll()
#include "Client.hpp"
#include "Color.hpp"
#include "Message.hpp"

class Client;
class User;

class Server {
private:
	int port; //server port
	int server_socketfd; //server socket file descriptor
	static bool signal; //static boolean to handle signals
	std::vector<Client> clients; //vector of clients
	std::vector<struct pollfd> fds; //vector of pollfd structures
	std::map<int, Client> users; //map of users
	std::map<int, std::string> nickname; //map of nicknames


public:
	Server();
	~Server();
	void ServerInit(); //initialize server
	void ServerSocket(); //create server socket

	void makePoll(int socket);
	void AcceptNewClient(); //accept new client
	void ReceiveData(int fd); //receive data from client
	void SendData(int fd, std::string message, int size); //send data to client

	static void SignalHandler(int signum); //signal handler

	void CloseFds(); //close file descriptor
	void ClearClients(int fd); //clear clients
};

#endif
