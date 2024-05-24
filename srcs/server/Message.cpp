#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く*/
void Message::ParsePrefix(const std::string &message, int &i){
		i = 1; // skip ':'
		while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n')
		{
			prefix_.push_back(message[i]);
			i++;
		}
		// skip space after prefix
		while (message[i] == ' ')
			i++;
}

/*parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）*/
void Message::ParseCommand(const std::string &message, int &i){
	while (i < message.length() && message[i] != ' ' && message[i] != '\r' && message[i] != '\n') {
		command_.push_back(message[i]);
		i++;
	}
	// skip spaces after command
	while (i < message.length() && message[i] == ' ')
		i++;
}

/*parseParams : IRCメッセージのパラメータを解析 */
void Message::ParseParams(const std::string &message, int &i){
	while (i < message.length() && message[i] != '\r' && message[i] != '\n') {
		if (message[i] == ':') {
			i++; //skip ':'
			// add the rest of the line
			params_.push_back(message.substr(i));
			break;
		}
		std::string param;
		while (i < message.length() && message[i] != ' ' && message[i] != '\r' && message[i] != '\n'){
			param.push_back(message[i]);
			i++;
		}
		if (!param.empty())
			params_.push_back(param);
		while (i < message.length() && message[i] == ' ')
			i++; // skip spaces between params
	}
}

std::string Message::GetPrefix() const {
	return this->prefix_;
}

std::string Message::GetCommand() const {
	return this->command_;
}

std::vector<std::string> Message::GetParams() const {
	return this->params_;
}

// メッセージバッファをクリアする
void Message::Clear() {
	prefix_.clear();
	command_.clear();
	params_.clear();
}
