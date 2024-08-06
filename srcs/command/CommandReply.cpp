#include "Command.hpp"

/* Welcomeメッセージを送信する関数
 * 引数1 -> クライアント */
void SendWelcomeMessage(Client &client)
{
	const int &fd = client.GetFd();
	const std::string &nick = client.GetNickname();

	SendMessage(fd, RPL_WELCOME(nick), 0);
	SendMessage(fd, RPL_YOURHOST(nick), 0);
	SendMessage(fd, RPL_CREATED(nick), 0);
	SendMessage(fd, RPL_MYINFO(nick), 0);

	client.SetIsWelcome(true);
}
