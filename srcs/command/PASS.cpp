#include "Command.hpp"

/* PASSコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーパスワード
 * 引数3 -> メッセージ */
void Command::PASS(Client &client, const std::string &server_password, const Message &message) {
	int const &fd = client.GetFd();
	const std::string &nick = client.GetNickname();

	// パラメータが1つでない場合
	if (message.GetParams().size() != 1){
		SendMessage(fd, ERR_NEEDMOREPARAMS(nick, "PASS"), 0);
		return ;
	}

	// パスワードを取得
	std::string const &password = message.GetParams()[0];

	// パスワードが正しい場合
	if (password == server_password) {
		client.SetIsAuthenticated(); // 認証済みに設定
	}
}


void SendMessage(int fd, const std::string &message, int flag) {
	send(fd, message.c_str(), message.size(), flag);
}

void SendMessageSSL(WOLFSSL* ssl, const std::string &message){
	if(ssl == NULL){
		std::cerr << "SSL session is not established." << std::endl;
		return ;
	}
	int len = wolfSSL_write(ssl, message.c_str(), message.size());
    if (len != message.size()) {
        std::cerr << "Failed to send all bytes over SSL. Error: " << wolfSSL_get_error(ssl, len) << std::endl;
    }
}


