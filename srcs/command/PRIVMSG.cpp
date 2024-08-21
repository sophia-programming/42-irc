#include "Command.hpp"

/* PRIVMSG <target> :<message> */

/* PRIVMSGコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::PRIVMSG(Client &client, Server *server, const Message &message) {
	std::vector<std::string> msg = message.GetParams();

	// パラメータが不足している場合はエラーメッセージを送信
	if (!IsCorrectFormat(msg, client))
		return;

	const std::string recipients =RmRFromString(msg[0]);
	const std::string content = RmRFromString(msg[1]);

	if (content.empty()){ // エラー　メッセージが空
		SendMessage(client.GetFd(), ERR_NOTEXTTOSEND(client.GetNickname()), 0);
		return;
	}

	// recipientsをカンマで分割
	std::stringstream ss(recipients);
	std::string recipient;

	while (std::getline(ss, recipient, ',')) {
		// 空のターゲットは無視する
		if (recipient.empty()) {
			continue;
		}

		if (recipient[0] == '#') {  // チャンネルに対して送信
			Channel* channel = server->FindChannelByName(recipient);
			if (!channel) { // エラー　一致するチャンネルが存在しない
				SendMessage(client.GetFd(), ERR_NOSUCHCHANNEL(client.GetNickname()), 0);
				continue;
			}
			channel->SendMsgToAll(content, &client);
		} else {  // ユーザーに対して送信
			Client *recep_client = server->FindClientByNickname(recipient);
			if (!recep_client) { // エラー　一致するクライアントが存在しない
				SendMessage(client.GetFd(), ERR_NOSUCHNICK(client.GetNickname(), recipient), 0);
				continue;
			}
			SendMessage(recep_client->GetFd(), content, 0);
		}
	}
}


/* 正しいフォーマットかどうかを確認する関数
 * 引数1 -> パラメータのvector
 * 引数2 -> クライアント */
bool IsCorrectFormat(std::vector<std::string> const &params, Client &client) {
	if (params.empty()) {
		SendMessage(client.GetFd(), ERR_NEEDMOREPARAMS(client.GetNickname(), "PRIVMSG"), 0);
		return false;
	}
	else if (params.size() < 2) {
		SendMessage(client.GetFd(), ERR_NOTEXTTOSEND(client.GetNickname()), 0);
		return false;
	}
	else
		return true;
}
