#include "Client.hpp"

Client::Client() {}

Client::Client(int fd, const std::string &nick) : FD(fd), nickname(nick) {}

Client::~Client() {}

/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く
 * parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * parseParams : IRCメッセージのパラメータを解析 */
void Client::parse() {
	int i = 0;

	if (message_buffer[i] == ':')
		parsed_msg.parsePrefix(message_buffer, i);
	parsed_msg.parseCommand(message_buffer, i);
	parsed_msg.parseParams(message_buffer, i);
}

void Client::addMessage(const std::string &message) {
	this->message_buffer += message;
}

const std::string &Client::getMessage() const {
	return (this->message_buffer);
}

int Client::getFD() const {
	return this->FD;
}

void Client::setFD(int fd) {
	this->FD = fd;
}

void Client::SetIPAddress(const std::string &ipaddress) {
	this->IPaddress = ipaddress;
}
