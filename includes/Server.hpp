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
#include <stdlib.h> // for exit()
#include <string> // for std::string
#include "poll.h" // for poll()
#include "Client.hpp"
#include "Color.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include <signal.h> // for SIGINT in linux


const int TIMEOUT = 300 * 1000; // 5 minutes in milliseconds

class Channel;
class Client;

bool ValidateArgs(int argc, char **argv);
std::string Trim(const std::string &str);

class Server {
private:
	int port_; //server port
	std::string password_; //server password
	std::vector<Client> connected_clients; //vector of clients
	int server_socket_fd_; //server socket file descriptor
	static bool signal_; //static boolean to handle signals
	std::vector<struct pollfd> fds_; //vector of pollfd structures
	std::map<int, Client> users_; //map of users
	std::map<int, std::string> nickname_; //map of nicknames
	std::map<std::string, int> map_nick_fd_; //map of nicknames and file descriptors

	void SetupServerSocket(); //create server socket
	void AcceptNewClient(); //accept new client
	void ChatFlow(int fd); //receive data from client
	void SendData(int fd, std::string message, int size); //send data to client
	void ClearClients(int fd); //clear clients
	void CloseFds(); //close file descriptor
	void MakePoll(int socket);
	void SetupClient(int socketfd);

	//channelのリスト(検索高速化の為に一旦mapで設定)
	// 1:channel namel -> チャンネル名の文字列
	// 2: channel class -> チャンネルオブジェクト
	typedef std::map<std::string, Channel*>::iterator channel_iterator;
	std::map<std::string, Channel*> channel_list_;


public:
	Server();
	Server(int port, const std::string &password);
	~Server();

	void ServerInit(int port); //initialize server
	void ServerStart(); //start server
	void Shutdown(); //shutdown server

	/* Signal */
	void SetupSignal(); //setup signal
	static void SignalHandler(int signum); //signal handler
	void Cleanup(int exitStatus); //cleanup server

	/* getter */
	int GetPort() const;
	std::string GetPassword() const;
	std::map<int, Client> GetUsers();
	int GetServerSocketFd() const;
	Channel* GetChannel( const std::string& name);
	Channel* CreateChannel( const std::string& name);
	bool IsChannel(const std::string& name);

	/* setter */
	void SetPassword(const std::string &password);

	/* Command */
	void ExecuteCommand(int fd, const Message &message);

	class ServerException : public std::exception{
			private:
				std::string msg_;
			public:
				ServerException(const std::string &msg):msg_(msg){};
				virtual ~ServerException () throw() {};
				virtual const char* what (void) const throw();
	};

};

#endif
