#include "User.hpp"

User::User() {}

User::User(int fd, const std::string &nick) : fd(fd), nickname(nick) {}

User::~User() {}

/* parsePrefixは、IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く
 * parseCommandは、IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * parseParamsは、IRCメッセージのパラメータを解析 */
void User::parse() {
	int i = 0;

	if (message_buffer[i] == ':')
		parsed_msg.parsePrefix(message_buffer, i);
	parsed_msg.parseCommand(message_buffer, i);
	parsed_msg.parseParams(message_buffer, i);
}

void User::addMessage(const std::string &message) {
	this->message_buffer += message;
}

const std::string &User::getMessage() const {
	return (this->message_buffer);
}
