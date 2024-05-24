#include "Client.hpp"

Client::Client() : fd_(-1), authenticated_(false) {}

Client::Client(int fd_, const std::string &nick) : fd_(fd_), nickname_(nick) {}

Client::~Client() {}

/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く
 * parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * parseParams : IRCメッセージのパラメータを解析 */
void Client::Parse(const std::string &message) {
	int i = 0;

	if (message[i] == ':')
		parsed_msg_.ParsePrefix(message, i);
	parsed_msg_.ParseCommand(message, i);
	parsed_msg_.ParseParams(message, i);

	std::cout << "======= [parsed message] ========" << std::endl;
	std::cout << "prefix: " << parsed_msg_.GetPrefix() << std::endl;
	std::cout << "command: " << parsed_msg_.GetCommand() << std::endl;
	for(size_t j = 0; j < parsed_msg_.GetParams().size(); j++)
		std::cout << "param[" << j << "]: " << parsed_msg_.GetParams()[j] << std::endl;
	std::cout << "================================" << std::endl;
	this->ClearMessage();
}

void Client::AddMessage(const std::string &message) {
	this->message_buffer_ += message;
	size_t pos = 0;

	// メッセージ終端を探す
	while ((pos = this->message_buffer_.find("\r\n")) != std::string::npos) {
		std::string single_message = this->message_buffer_.substr(0, pos);
		this->Parse(single_message);
		this->message_buffer_.erase(0, pos + 2); // メッセージをバッファから削除
	}
}

const std::string &Client::GetMessage() const {
	return (this->message_buffer_);
}

int Client::GetFd() const {
	return this->fd_;
}

std::string Client::GetNickname() const {
	return this->nickname_;
}

std::string Client::GetIpAdress() const {
	return this->ip_address_;
}

bool Client::IsAuthenticated() const {
	return this->authenticated_;
}

void Client::SetFd(int fd) {
	this->fd_ = fd;
}

void Client::SetIPAddress(const std::string &ipaddress) {
	this->ip_address_ = ipaddress;
}

void Client::Authenticate() {
	this->authenticated_ = true;
}

void Client::SetNickname(const std::string &nick) {
	this->nickname_ = nick;
}

// メッセージバッファをクリアする
void Client::ClearMessage() {
	Message newParsedMsg;

	this->message_buffer_ = "";
	this->parsed_msg_ = newParsedMsg;
}
