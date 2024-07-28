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

	// パラメータがない場合はデフォルトのメッセージ(Goodbye)を使用
	std::string quitMessage = params.empty() ? "Goodbye" : params[0];

	// チャンネル内の他のメンバーにQUITメッセージを送信
	const std::map<std::string, Channel*>& channels = server->GetChannels();
	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;

		// チャンネル内にクライアントがいるかどうかを確認
		Client* channelMember = channel->GetUser(nick);
		if (channelMember != NULL) {
			std::cout << "Client " << nick << " is a member of channel " << channel->GetName() << std::endl;

			// チャンネル内の他のメンバーにQUITメッセージを送信
			std::string messageContent = QUIT_MESSAGE(nick, client.GetUsername(), client.GetHostname(), quitMessage);
			for (std::map<Client*, User_Priv>::const_iterator member_it = channel->users_.begin(); member_it != channel->users_.end(); ++member_it) {
				Client *member = member_it->first;
				if (member->GetFd() != clientFd) {
					SendMessage(member->GetFd(), messageContent, 0);
					std::cout << "Sent QUIT message to member: " << member->GetNickname() << std::endl;
				}
			}

			channel->RmUser(&client);
			std::cout << "Removed client " << nick << " from channel " << channel->GetName() << std::endl;
		} else {
			std::cout << "Client " << nick << " is not a member of channel " << channel->GetName() << std::endl;
		}
	}

	// クライアントにエラーメッセージを送信
	std::string errorMessage = "ERROR :Closing Link: " + client.GetHostname() + " (" + quitMessage + ")\r\n";
	SendMessage(clientFd, errorMessage, 0);
	std::cout << "Sent error message to client: " << errorMessage << std::endl;

	// クライアントの接続を終了し、関連するリソースをクリーンアップ
	try {
		ClearClientInfo(client, pollfds, users, nick_to_fd);
		std::cout << "Cleared client info for: " << nick << std::endl;
	} catch (const std::exception &e) {
		std::cerr << "Error during client cleanup: " << e.what() << std::endl;
	}

	server->RemoveClient(clientFd);
	std::cout << "Removed client from server: " << nick << std::endl;
}
