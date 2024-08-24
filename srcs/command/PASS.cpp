#include "Command.hpp"

/* PASSコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーパスワード
 * 引数3 -> メッセージ */
void Command::PASS(Client &client,const std::string &server_password, const Message &message) {
	int const &fd = client.GetFd();
	const std::string &nick = client.GetNickname();
	std::vector<std::string> msg = message.GetParams();

	// パラメータが1つでない場合
	if (msg.size() != 1){
		SendMessage(fd, ERR_NEEDMOREPARAMS(nick, "PASS"), 0);
		return ;
	}

	// パスワードを取得
	std::string const &password = RmRFromString(msg[0]);

	// パスワードが正しい場合
	if (password == server_password) {
		client.SetIsAuthenticated(); // 認証済みに設定
	}else{
		SendMessage(client.GetFd(),ERR_PASSWDMISMATCH(client.GetNickname()),0);
	}
}


void SendMessage(int fd, const std::string &message, int flag) {
	send(fd, message.c_str(), message.size(), flag);
}
