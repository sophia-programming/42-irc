#include "Client.hpp"

Client::Client() : fd_(-1), is_authenticated_(false) {}

Client::Client(int fd_, const std::string &nick)
	: fd_(fd_), nickname_(nick), is_authenticated_(false), is_nickname_(true), is_user_set_(false) {}

Client::~Client() {}


/* ===== メッセージ処理関数 ===== */
/* メッセージをバッファに追加する関数
 * 引数1 -> メッセージ */
void Client::AddMessage(const std::string &message) {
	this->message_buffer_ += message;
}

// mapに格納するために必要な比較演算子のオーバーロード
bool Client::operator<(const Client &other) const
{
    return this->nickname_ < this->nickname_;
}

/* メッセージをパースする関数
 * 引数1 -> メッセージ */
void Client::Parse(const std::string &message) {
	int i = 0;
	/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　(⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く)
 * parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * parseParams : IRCメッセージのパラメータを解析 */
	parsed_msg_.ParsePrefix(message, i);
	parsed_msg_.ParseCommand(message, i);
	parsed_msg_.ParseParams(message, i);
	Debug_parser();
}

/* メッセージバッファをクリアする関数 */
void Client::ClearMessage() {
	parsed_msg_.Clear();
}


/* ====== getter関数 ====== */
int Client::GetFd() const {
	return this->fd_;
}

bool Client::GetIsAuthenticated() const {
	return this->is_authenticated_;
}

const std::string &Client::GetNickname() const {
	return this->nickname_;
}

bool Client::GetIsWelcome() const {
	return this->is_welcome_;
}

bool Client::GetIsConnected() const {
	return this->is_connected_;
}

std::string Client::GetIpAdress() const {
	return this->ip_address_;
}

std::string &Client::GetMessage(){
	return this->message_buffer_;
}

bool Client::GetIsNick() const{
	return this->is_nickname_;
}

const std::string &Client::GetUsername() const {
	return this->username_;
}

const std::string &Client::GetHostname() const {
	return this->hostname_;
}

const std::string &Client::GetServername() const {
	return this->servername_;
}

const std::string &Client::GetRealname() const {
	return this->realname_;
}

bool Client::GetIsUserSet() const {
	std::cout << "is_user_set_ : " << this->is_user_set_ << std::endl;
	return this->is_user_set_;
}

const Message& Client::GetParsedMessage() const {
	return parsed_msg_;
}

/* ====== setter関数 ====== */
void Client::SetFd(int fd) {
	this->fd_ = fd;
}

void Client::SetIsAuthenticated() {
	this->is_authenticated_ = true;
}

void Client::SetNickname(const std::string &nick) {
	this->nickname_ = nick;
	this->is_user_set_ = true;
}

void Client::SetIsWelcome(bool is_welcome) {
	this->is_welcome_ = is_welcome;
}

void Client::SetIsConnected(bool is_connected) {
	this->is_connected_ = is_connected;
}

void Client::SetIPAddress(const std::string &ipaddress) {
	this->ip_address_ = ipaddress;
}

void Client::SetIsNick() {
	this->is_nickname_ = true;
}

void Client::SetUsername(const std::string &username) {
	this->username_ = username;
}

void Client::SetHostname(const std::string &hostname) {
	this->hostname_ = hostname;
}

void Client::SetServername(const std::string &servername) {
	this->servername_ = servername;
}

void Client::SetRealname(const std::string &realname) {
	this->realname_ = realname;
}

void Client::SetIsUserSet(bool flag) {
	this->is_user_set_ = flag;
}


/* ====== 認証関数 ====== */

void Client::Debug_parser() {
	std::cout << "======= [parsed message] ========" << std::endl;
	std::cout << "prefix: " << parsed_msg_.GetPrefix() << std::endl;
	std::cout << "command: " << parsed_msg_.GetCommand() << std::endl;
	for(size_t j = 0; j < parsed_msg_.GetParams().size(); j++)
		std::cout << "param[" << j << "]: " << parsed_msg_.GetParams()[j] << std::endl;
	std::cout << "================================" << std::endl;
	this->ClearMessage();
}
