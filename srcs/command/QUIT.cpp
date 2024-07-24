#include "Command.hpp"

/* QUIT <message> */

/* QUITコマンド(クライアントを切断する)
 * 引数1 -> クライアント
 * 引数2 -> サーバー
 * 引数3 -> pollfd構造体のリスト
 * 引数4 -> ユーザーリスト
 * 引数5 -> ニックネームとファイルディスクリプタのマップ
 * 引数6 -> メッセージのパラメータ
 * 引数7 -> メッセージオブジェクト */

void Command::QUIT(Client &client, Server *server, std::vector<struct pollfd> &pollfds,
		  std::map<int, Client> &users, std::map<std::string, int> &nick_to_fd,
		  const std::vector<std::string> &params, const Message &message) {
	const std::string nick = client.GetNickname();
	const int &ClientFd = client.GetFd();

	// QUITメッセージがある場合はそれを使う.今回はないのでデフォルトで"Goodbye"を使う
	std::string QuitMessage = "Goodbye";

	// メッセージがある場合はそれを使う
	if (!params.empty())
		QuitMessage = message.GetParams()[0];

	// チャンネルのメンバーにQUITメッセージを送信.
	const std::map<std::string, Channel*>& channels = server->GetChannels();
	if (!channels.empty())
	{
		for (std::map<std::string, Channel*>::const_iterator it = channels.begin();
			 it != channels.end();
			 ++it) {
			const std::vector<Client *> &members = it->second->GetMember();
			for (std::vector<Client *>::const_iterator members_it = members.begin();
				 members_it != members.end();
				 ++members_it) {
				const int membersFd = (*members_it)->GetFd();

				// メンバーがQUITしたクライアントの場合はメンバーから削除
				if (membersFd == ClientFd)
					it->second->RmUser(&client);

				// メンバーにQUITメッセージを送信
				else {
					std::string messageContent = QUIT_MESSAGE(nick, client.GetUsername(), client.GetHostname(), QuitMessage);
					SendMessage(membersFd, messageContent, 0);
				}
			}
		}
	}

	// クライアントの接続を終了し、関連するリソースをクリーンアップ
	ClearClientInfo(client, pollfds, users, nick_to_fd);
}
