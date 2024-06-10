#include "Command.hpp"


/* USERコマンドの処理を行う関数
 * 引数1 -> クライアント
 * Command: USER
 * Parameters: <username> <hostname> <servername> <realname>*/

void Command::USER(Client &client, const Message &message) {
	int fd = client.GetFd();
	std::string nickname = client.GetNickname();

	if (message.GetParams().size() != 4)
		SendMessage(fd, std::string(YELLOW) + ERR_NEEDMOREPARAMS(nickname, "USER") + std::string(STOP), 0);
	else {
		client.SetUsername(message.GetParams()[0]);
		client.SetHostname(message.GetParams()[1]);
		client.SetServername(message.GetParams()[2]);
		client.SetRealname(message.GetParams()[3]);

		// ログ出力
		std::cout << "Username: " << client.GetUsername() << std::endl;
		std::cout << "Hostname: " << client.GetHostname() << std::endl;
		std::cout << "Servername: " << client.GetServername() << std::endl;
		std::cout << "Realname: " << client.GetRealname() << std::endl;
	}
}
