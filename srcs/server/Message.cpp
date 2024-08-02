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
	// i = 1は、メッセージの最初がコロン : で始まるため、その次の文字から開始するため
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
