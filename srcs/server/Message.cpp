#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

Message::Message(const std::string &message) : original_message_(message) {
	size_t i = 0;
	ParsePrefix(original_message_, i);
	ParseCommand(original_message_, i);
	ParseParams(original_message_, i);
}

/* IRCメッセージのプレフィックス（例: :irc.example.com）を解析する関数
 *（⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く）
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParsePrefix(const std::string &message, size_t &i) {
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
void Message::ParseCommand(const std::string &message, size_t &i) {
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
	//受け取ったコマンドを全て大文字にする
	for(size_t j=0; j<this->command_.size(); j++){
		command_[j] = std::toupper(command_[j]);
	}
}


//stringの末尾の改行を削除する関数
// 1:std::string& str ->対象のstring
void rtrim(std::string& str) {
    std::string::size_type i = str.find_last_not_of("\n");
    if (i == std::string::npos) {
        str.clear(); // 全て空白文字の場合は空にする
    } else {
        str.erase(i + 1); // 最後の非空白文字の次を削除
    }
}

/*IRCメッセージのパラメータを解析する関数
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParseParams(const std::string &message, size_t &i) {
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
	if(!params_.empty()){
		rtrim(params_.back());
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
