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

	const std::string &nick = client.GetNickname();
	const int &clientFd = client.GetFd();

	// もしQUITメッセージが指定されていない場合は、デフォルトのメッセージを使用
	std::string quitMessage = "Goodbye";
	if (!params.empty()) {
		quitMessage = message.GetParams()[0];
	}

	// クライアントが参加しているチャンネルのメンバーにのみQUITメッセージを送信
	if (!server->GetChannels().empty()) {
		const std::map<std::string, Channel*>& channels = server->GetChannels();
		std::map<std::string, Channel*>::const_iterator it;
		// debug
		for (auto it = channels.begin(); it != channels.end(); ++it) {
			std::cout << BOLD << "channels = " << it->first << STOP << std::endl;
		}

		// チャンネルのメンバーにQUITメッセージを送信
		for (it = channels.begin(); it != channels.end(); ++it) {
			Channel* channel = it->second;
			// 上記ポインタが入っているのを確認済み

			//======================== ここから機能していない =======================

			// チャンネルにクライアントが参加している場合
			if (channel->IsMember(&client)) {
				const std::vector<Client *> &members = channel->GetMember();
				for (auto it = members.begin(); it != members.end(); ++it) {
					std::cout << BOLD << (*it)->GetUsername() << STOP << std::endl;
				}

				std::vector<Client *>::const_iterator member_it;
				for (member_it = members.begin(); member_it != members.end(); ++member_it) {
					Client *member = *member_it;

					for (auto it = members.begin(); it != members.end(); ++it) {
						std::cout << BOLD << (*it)->GetFd() << STOP << std::endl;
					}
					std::cout << BOLD << "clientFd " << clientFd << STOP << std::endl;

//				 もしメンバーがクライアント自身でない場合は、QUITメッセージを送信
					if (member->GetFd() != clientFd) {
						std::cout << BOLD << "client.GetUsername() " << client.GetUsername() << STOP << std::endl;
						std::string messageContent = QUIT_MESSAGE(nick, client.GetUsername(), client.GetHostname(), quitMessage);
						SendMessage(member->GetFd(), messageContent, 0);
					}

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

