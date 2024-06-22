#include "Command.hpp"

void ClearClientInfo(
		Client &client,
		std::vector<struct pollfd> &pollfds,
		std::map<int, Client> &users,
		std::map<std::string, int> &nick_to_fd
);

/* CAPコマンド(クライアントがサポートする機能をリストアップする)
 * 引数1 -> クライアント
 * 引数2 -> pollfd構造体のvector
 * 引数3 -> ユーザーのmap
 * 引数4 -> ニックネームとファイルディスクリプタのmap
 * 引数5 -> メッセージ */
void Command::CAP(Client &client, std::vector<struct pollfd> &pollfds,
				   std::map<int, Client> &users, std::map<std::string, int> &nick_to_fd,
				   const Message &message) {
	const std::vector<std::string> &params = message.GetParams();
	const std::string nick = client.GetNickname();
	const int &fd = client.GetFd();

	if (params[0] == "LS")
		SendMessage(fd, CAP_LS, 0);
	else if (params[0] == "END") {
		if (!client.GetIsAuthenticated()) {
			SendMessage(fd, ERR_PASSWDMISMATCH(nick), 0);
			SendMessage(fd, PASS_ERROR(client.GetHostname()), 0);
			ClearClientInfo(client, pollfds, users, nick_to_fd);
			return;
		}
		SendMessage(fd, RPL_NONE((std::string) "Authenticated ..."), 0);

		if (client.GetIsNick()) {
			client.SetIsConnected(true);
			SendWelcomeMessage(client);
		}
	}
}


/* クライアントのデータを削除する関数
 * 引数1 -> クライアントのソケットファイルディスクリプタ
 * 引数2 -> 送信するメッセージ
 * 引数3 -> 送信するメッセージのサイズ */
void ClearClientInfo(
		Client &client,
		std::vector<struct pollfd> &pollfds,
		std::map<int, Client> &users,
		std::map<std::string, int> &nick_to_fd
) {
	const std::string nick = client.GetNickname();

	for (std::vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it++) {
		if (client.GetFd() == it->fd) {
			pollfds.erase(it);
			break;
		}
	}
	users.erase(client.GetFd());
	nick_to_fd.erase(nick);
	close(client.GetFd());
}
