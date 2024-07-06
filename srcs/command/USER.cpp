#include "Command.hpp"

/* USER <username> <hostname> <servername> <realname> */

/* USERコマンド(ユーザー情報を設定する)
 * 引数1 -> クライアント
 * 引数2 -> メッセージ */

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
	}
}
