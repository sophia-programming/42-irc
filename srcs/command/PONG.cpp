#include "Command.hpp"

/* PONG <server1> [ <server2> ] */

/* PONGコマンド(クライアントに対してPONGメッセージを返す)
 * 引数1 -> クライアント
 * 引数2 -> params	*/
void Command::PONG(Client &client, const std::vector <std::string> &params) {
	const int &fd = client.GetFd();
	const std::string &ServerName = params[0];

	if (params.empty()) {
		SendMessage(fd, std::string(YELLOW) + ERR_NEEDMOREPARAMS(client.GetNickname(), "PONG") + std::string(STOP), 0);
		return;
	}

	SendMessage(fd, PONG_MESSAGE(ServerName), 0);
}
