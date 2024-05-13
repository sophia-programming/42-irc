#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く*/
void Message::parsePrefix(const std::string &message, int &i){
		i = 1;
		while (message[i] != ' ') //parse prefix
		{
			prefix.append(&message[i], 1);
			i++;
		}
		while (message[i] == ' ')
			i++;
}

/*parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）*/
void Message::parseCommand(const std::string &message, int &i){
	while (message[i] != '\r' && message[i] != '\n' && message[i] != ' ')
		command.append(&message[i], 1);
	while (message[i] == ' ')
		i++;
}

/*parseParams : IRCメッセージのパラメータを解析 */
void Message::parseParams(const std::string &message, int &i){
	while (message[i] != '\r' && message[i] != '\n')
	{
		std::string param;
		while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n')
		{
			param.append(&message[i], 1);
			i++;
		}
		params.push_back(param);
		while (message[i] == ' ')
			i++;
	}
}
