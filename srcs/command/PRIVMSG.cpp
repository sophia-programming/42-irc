#include "Command.hpp"

/* PRIVMSG <receiver> :<message> */

bool IsCorrectFormat(std::vector<std::string> const &params, Client &client);
void SendToChannel(Client &client, const std::string &channel, const std::string &message);
void SendToUser(Client &client, const std::string &target, const std::string &message);


/* PRIVMSGコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> パラメータのvector */
void Command::PRIVMSG(Client &client, const std::vector<std::string> &params) {
	if (!IsCorrectFormat(params, client))
		return;

	std::string target = params[0];
	std::string message = params[1];
	for (size_t i = 2; i < params.size(); ++i) {
		message += " " + params[i];
	}
	// メッセージ先頭のコロンを削除
	if (message[0] == ':')
		message = message.substr(1);

	// channelは#から始まる
	if (target[0] == '#')
		SendToChannel(client, target, message);
	else
		SendToUser(client, target, message);
}

/* 正しいフォーマットかどうかを確認する関数
 * 引数1 -> パラメータのvector
 * 引数2 -> クライアント */
bool IsCorrectFormat(std::vector<std::string> const &params, Client &client) {
	if (params.empty()) {
		SendMessage(client.GetFd(), ERR_NEEDMOREPARAMS(client.GetNickname(), "PRIVMSG"), 0);
		return false;
	}
	else if (params.size() < 2) {
		SendMessage(client.GetFd(), ERR_NOTEXTTOSEND(client.GetNickname()), 0);
		return false;
	}
	else
		return true;
}


/* チャンネルへのメッセージ送信
 * 引数1 -> クライアント
 * 引数2 -> チャンネル名
 * 引数3 -> メッセージ */
void SendToChannel(Client &client, const std::string &channel, const std::string &message) {
}


/* ユーザーへのメッセージ送信
 * 引数1 -> クライアント
 * 引数2 -> ユーザー名
 * 引数3 -> メッセージ */
void SendToUser(Client &client, const std::string &target, const std::string &message) {
}
