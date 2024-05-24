#include "Command.hpp"

/* PASSコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーパスワード*/
void PASS(Client &client, const std::string &server_password, const Message &message) {
	Server server;
	int fd = client.GetFd();
	const std::string &nick = client.GetNickname();

	// パラメータが1つでない場合
	if (message.GetParams().size() != 1) {
		SendMessage(fd, ERR_NEEDMOREPARAMS(nick, "PASS"), 0);
		return ;
	}

	// パスワードが正しい場合
	std::string const &password = message.GetParams()[0];

	if (password == server_password)
		client.IsAuthenticated();
}

void SendMessage(int fd, const std::string &message, int flag) {
	send(fd, message.c_str(), message.size(), flag);
	std::cout << YELLOW << "Sent: " << message << STOP << std::endl;
}
