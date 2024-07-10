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
	i = 1;

	// プレフィックスがない場合(プレフィックスはコロン : で始まり、最初の空白文字 まで続く)
	while (message[i] != ' ') {
		prefix_.append(&message[i], 1);
		i++;
	}

	// スペースをスキップ
	while(message[i] == ' ')
		i++;
}


/*IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParseCommand(const std::string &message, int &i) {

	// message[i]がスペースでない場合、コマンドを取得
	while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n')
	{
		command_.append(&message[i], 1);
		i++;
	}

	// スペースをスキップ
	while (message[i] == ' ')
		i++;
}


/*IRCメッセージのパラメータを解析する関数
 * 引数1 -> メッセージ
 * 引数2 -> メッセージのインデックス*/
void Message::ParseParams(const std::string &message, int &i) {

	// パラメータがない場合
	while (message[i] != '\r' && message[i] != '\n' && message[i] != '\0')
	{
		std::string param;

		// パラメータがコロンで始まる場合(例: :irc.example.com), パラメータを取得
		if (message[i] == ':')
		{
			i += 1;
			while (message[i] != '\r' && message[i] != '\n')
			{
				param.append(&message[i], 1);
				i++;
			}
		}

		// message[i]がスペースでない場合、パラメータを取得
		while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n')
		{
			param.append(&message[i], 1);
			i++;
		}

		// スペースをスキップ
		i++;

		// パラメータを追加
		params_.push_back(param);
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
