#include "Server.hpp"

void Client::setNickname(const std::string &nick) {
	this->nickname = nick;
}

void Client::setUsername(const std::string &user) {
	this->username = user;
}

void Server::handleCommand(int clientID, const std::string &command, const std::string &args) {
	if (command == "NICK") {
		clients[clientID].setNickname(args);
	} else if (command == "USER") {
		clients[clientID].setUsername(args);
	}
	// 他のコマンドの処理
}
