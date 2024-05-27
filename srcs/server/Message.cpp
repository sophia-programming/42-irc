#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

Message::Message(const std::string &message) : original_message_(message) {
	int i = 0;
	ParsePrefix(original_message_, i);
	ParseCommand(original_message_, i);
	ParseParams(original_message_, i);
}

/* IRCメッセージのプレフィックス（例: :irc.example.com）を解析する関数
 *（⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く）
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParsePrefix(const std::string &message, int &i) {
	if (message[i] == ':') {
		i++;
		size_t pos = message.find(' ', i);
		if (pos != std::string::npos) {
			prefix_ = message.substr(i, pos - i);
			i = pos + 1;
		}
		// スペースをスキップ
		while (i < message.size() && message[i] == ' ')
			i++;
	}
}


/*IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParseCommand(const std::string &message, int &i) {
	size_t pos = message.find(' ', i);
	if (pos != std::string::npos) {
		command_ = message.substr(i, pos - i);
		i = pos + 1;
	} else {
		command_ = message.substr(i);
		i = message.size();
	}
	// スペースをスキップ
	while (i < message.size() && message[i] == ' ')
		i++;
}


/*IRCメッセージのパラメータを解析する関数
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParseParams(const std::string &message, int &i) {
	while (i < message.size() && message[i] != '\r' && message[i] != '\n') {
		if (message[i] == ':') {
			i++; // skip ':'
			params_.push_back(message.substr(i));
			break;
		}
		size_t pos = message.find(' ', i);
		if (pos != std::string::npos) {
			params_.push_back(message.substr(i, pos - i));
			i = pos + 1;
		} else {
			params_.push_back(message.substr(i));
			break;
		}
		// スペースをスキップ
		while (i < message.size() && message[i] == ' ')
			i++;
	}
}

/* メッセージバッファをクリアする関数 */
void Message::Clear() {
	prefix_.clear();
	command_.clear();
	params_.clear();
}


/* getter関数 */
std::string Message::GetPrefix() const {
	return prefix_;
}

std::string Message::GetCommand() const {
	return command_;
}

const std::vector<std::string> &Message::GetParams() const {
	return params_;
}
