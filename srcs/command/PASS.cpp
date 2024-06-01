#include "Command.hpp"

/* PASSコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::PASS(Client &client, Server *server, const Message &message) {
	int fd = client.GetFd();
	const std::string &nick = client.GetNickname();

	// パラメータが1つでない場合
	if (message.GetParams().size() != 1) {
		SendMessage(fd, std::string(YELLOW) + ERR_NEEDMOREPARAMS(nick, "PASS") + std::string(STOP), 0);
		return ;
	}

	// パスワードを取得
	std::string password = message.GetParams()[0];
	password = Trim(password);

	std::string server_password = server->GetPassword();
	server_password = Trim(server_password);

	// パスワードが正しい場合
	if (password == server_password) {
		client.Authenticate(); // 認証する
		SendMessage(fd, std::string(GREEN) + "Welcome! You are now authenticated.\r\n" + std::string(STOP), 0);
		//一旦プログラムを終了させる
		exit(0);
	} else {
		// パスワードが正しくない場合
		SendMessage(fd, std::string(RED) + "Password is incorrect.\r\n" + std::string(STOP), 0);
	}
}


void SendMessage(int fd, const std::string &message, int flag) {
	send(fd, message.c_str(), message.size(), flag);
}
