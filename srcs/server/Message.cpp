#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

/* parsePrefix : IRCメッセージのプレフィックス（例: :irc.example.com）を解析
 *　⇨プレフィックスはコロン : で始まり、最初の空白文字 まで続く*/
void Message::parsePrefix(const std::string &message, int &i){
		i = 1; // skip ':'
		while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n')
		{
			prefix.append(&message[i], 1);
			i++;
		}
		while (message[i] == ' ') // skip space after prefix
			i++;
}

/*parseCommand : IRCメッセージの操作を指示するキーワード（例: PRIVMSG, JOIN など）*/
void Message::parseCommand(const std::string &message, int &i){
	while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n') {
		command.append(&message[i], 1);
		i++;
	}
	while (message[i] == ' ') // skip spaces after command
		i++;
}

/*parseParams : IRCメッセージのパラメータを解析 */
void Message::parseParams(const std::string &message, int &i){
	while (message[i] != '\r' && message[i] != '\n') {
		if (message[i] == ':') {
			i++; //skip ':'
			params.push_back(message.substr(i)); // add the rest of the line
			break;
		}
		std::string param;
		while (message[i] != ' ' && message[i] != '\r' && message[i] != '\n'){
			param.append(&message[i], 1);
			i++;
		}
		params.push_back(param);
		while (message[i] == ' ')
			i++; // skip spaces between params
	}
}
