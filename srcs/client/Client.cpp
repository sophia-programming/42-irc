#include "Client.hpp"

Client::Client() : fd_(-1), is_authenticated_(false) {}

Client::Client(int fd_, const std::string &nick)
	: fd_(fd_), is_authenticated_(false), is_nickname_(true) {}

Client::~Client() {}


/* ===== メッセージ処理関数 ===== */
/* メッセージをバッファに追加する関数
 * 引数1 -> メッセージ */
void Client::AddMessage(const std::string &message) {
	this->message_buffer_ += message;
	size_t pos = 0;

	// メッセージ終端を探す
	while ((pos = this->message_buffer_.find("\r\n")) != std::string::npos) {
		std::string single_message = this->message_buffer_.substr(0, pos);
		this->Parse(single_message);
		// メッセージをバッファから削除
		this->message_buffer_.erase(0, pos + 2);
	}
}

/* メッセージをパースする関数
 * 引数1 -> メッセージ */
void Client::Parse(const std::string &message) {
	int i = 0;

	/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　(⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く)
 * parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * parseParams : IRCメッセージのパラメータを解析 */
	if (message[i] == ':')
		parsed_msg_.ParsePrefix(message, i);
	parsed_msg_.ParseCommand(message, i);
	parsed_msg_.ParseParams(message, i);
	Debug_parser();
}

/* メッセージバッファをクリアする関数 */
void Client::ClearMessage() {
	parsed_msg_.Clear();
}


/* ====== setter関数 ====== */
void Client::SetFd(int fd) {
	this->fd_ = fd;
}

void Client::SetNickname(const std::string &nick) {
	this->nickname_ = nick;
}

void Client::SetIPAddress(const std::string &ipaddress) {
	this->ip_address_ = ipaddress;
}


/* ====== getter関数 ====== */
int Client::GetFd() const {
	return this->fd_;
}

std::string Client::GetNickname() const {
	return this->nickname_;
}

std::string Client::GetIpAdress() const {
	return this->ip_address_;
}

const std::string &Client::GetMessage() const {
	return (this->message_buffer_);
}


/* ====== 認証関数 ====== */
void Client::Authenticate() {
	this->is_authenticated_ = true;
}

bool Client::IsAuthenticated() const {
	return this->is_authenticated_;
}

void Client::Debug_parser() {
	std::cout << "======= [parsed message] ========" << std::endl;
	std::cout << "prefix: " << parsed_msg_.GetPrefix() << std::endl;
	std::cout << "command: " << parsed_msg_.GetCommand() << std::endl;
	for(size_t j = 0; j < parsed_msg_.GetParams().size(); j++)
		std::cout << "param[" << j << "]: " << parsed_msg_.GetParams()[j] << std::endl;
	std::cout << "================================" << std::endl;
	this->ClearMessage();
}
