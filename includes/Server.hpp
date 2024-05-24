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
#include "Channel.hpp"

class Client;
class User;
class Channel;

bool validate_args(int argc, char **argv);

class Server {
private:
	int port_; //server port
	int server_socket_fd_; //server socket file descriptor
	std::string password_; //server password
	static bool signal_; //static boolean to handle signals
	std::vector<Client> connected_clients; //vector of clients
	std::vector<struct pollfd> fds_; //vector of pollfd structures
	std::map<int, Client> users_; //map of users
	std::map<int, std::string> nickname_; //map of nicknames

	//channelのリスト(検索高速化の為に一旦mapで設定)
	// 1:channel namel -> チャンネル名の文字列
	// 2: channel class -> チャンネルオブジェクト
//	std::map<std::string, Channel> channel_list_;


public:
	Server();
	Server(int port, const std::string &password);
	~Server();
	void ServerInit(); //initialize server
	void ServerSocket(); //create server socket

	void MakePoll(int socket);
	void AcceptNewClient(); //accept new client
	void ReceiveData(int fd); //receive data from client
	void SendData(int fd, std::string message, int size); //send data to client

	static void SignalHandler(int signum); //signal handler

	void CloseFds(); //close file descriptor
	void ClearClients(int fd); //clear clients

	int GetPort() const;
	bool IsChannel(std::string& name);
	Channel GetChannel(std::string& name);
	void CreateChannel(std::string& name);
};

#endif
