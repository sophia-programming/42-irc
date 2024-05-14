#include "Client.hpp"

Client::Client() {}

Client::Client(int fd, const std::string &nick) : FD(fd), nickname(nick) {}

Client::~Client() {}

/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く
 * parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * parseParams : IRCメッセージのパラメータを解析 */
void Client::parse(const std::string &message) {
	int i = 0;

	if (message[i] == ':')
		parsed_msg.parsePrefix(message, i);
	parsed_msg.parseCommand(message, i);
	parsed_msg.parseParams(message, i);
	std::cout << "======= [parsed message] ========" << std::endl;
	std::cout << "prefix: " << parsed_msg.getPrefix() << std::endl;
	std::cout << "command: " << parsed_msg.getCommand() << std::endl;
	for(size_t i = 0; i < parsed_msg.getParams().size(); i++)
		std::cout << "param[" << i << "]: " << parsed_msg.getParams()[i] << std::endl;
	this->clearMessage(message.length());
}

void Client::addMessage(const std::string &message) {
	this->message_buffer += message;
	size_t pos = 0;

	// メッセージ終端を探す
	while ((pos = this->message_buffer.find("\r\n")) != std::string::npos) {
		std::string single_message = this->message_buffer.substr(0, pos);
		this->parse(single_message);
		this->message_buffer.erase(0, pos + 2); // メッセージをバッファから削除
	}
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

void Client::clearMessage(size_t length) {
	if (message_buffer.length() <= length) {
		message_buffer.clear(); // 全ての文字をクリア
	} else {
		message_buffer.erase(0, length); // 指定した長さのメッセージを削除
	}
}

