#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

/* プレフィックスの解析開始: parserPrefix()は、IRCクライアントがサーバーから受け取ったメッセージの中から
       プレフィックス（送信者の識別情報など）を抽出し、それを処理するために必要
       (メッセージの最初にコロン(:)で始まる部分) */
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

void Message::parseCommand(const std::string &message, int &i){
	while (message[i] != '\r' && message[i] != '\n' && message[i] != ' ')
		command.append(&message[i], 1);
	while (message[i] == ' ')
		i++;
}

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
