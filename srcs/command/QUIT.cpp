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

	// クライアントが接続していない場合は何もしない
	if (!client.GetIsWelcome() || !client.GetIsConnected()) {
		return;
	}

	const std::string &nick = client.GetNickname();
	const int &clientFd = client.GetFd();

	// もしQUITメッセージが指定されていない場合は、デフォルトのメッセージを使用
	std::string quitMessage = "Goodbye";
	if (!params.empty()) {
		quitMessage = message.GetParams()[0];
	}

	// クライアントが参加しているチャンネルのメンバーにのみQUITメッセージを送信
	const std::map<std::string, Channel*>& channels = server->GetChannels();
	std::map<std::string, Channel*>::const_iterator it;

	// チャンネルのメンバーにQUITメッセージを送信
	for (it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;

		// チャンネルにクライアントが参加している場合
		if (channel->IsMember(&client)) {
			const std::vector<Client *> &members = channel->GetMember();
			std::vector<Client *>::const_iterator member_it;
			for (member_it = members.begin(); member_it != members.end(); ++member_it) {
				Client *member = *member_it;

				// もしメンバーがクライアント自身でない場合は、QUITメッセージを送信
				if (member->GetFd() != clientFd) {
					std::string messageContent = QUIT_MESSAGE(nick, client.GetUsername(), client.GetHostname(), quitMessage);
					SendMessage(member->GetFd(), messageContent, 0);
				}
			}
			channel->RmUser(&client);
		}
	}

	// クライアントの接続を終了し、関連するリソースをクリーンアップ
	try {
		ClearClientInfo(client, pollfds, users, nick_to_fd);
	} catch (const std::exception &e) {
		std::cerr << "Error during client cleanup: " << e.what() << std::endl;
	}

	// サーバーからクライアントを削除
	server->RemoveClient(clientFd);
}

