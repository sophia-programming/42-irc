#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unordered_map> //c++11かも
#include <netdb.h>
#include <unistd.h>
#include "Color.hpp"
#include "Command.hpp"
#include "Client.hpp"


#define DEFAULT_PORT "6667"  // ポート番号は文字列として定義する必要がある

class Server {
private:
	std::unordered_map<int, Client> clients; // クライアントIDとクライアント情報のマッピング

public:
	int  initializeServer(const char* port, int &listenSocket);
	void handleConnections(int listenSocket, std::string password);
	void handleCommand(int clientID, const std::string &command, const std::string &args);
};

#endif
