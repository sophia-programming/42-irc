#include "Command.hpp"


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
