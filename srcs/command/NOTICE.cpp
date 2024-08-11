#include "Command.hpp"

/* NOTICE <recipient> :<content> */

/* NOTICEコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::NOTICE(Client &client, Server *server, const Message &message) {
	const std::vector<std::string> msg = message.GetParams();
	std::string msg_to_c;

	if (!IsCorrectFormat(msg, client))
		return;

	// パラメータが不足している場合は処理を終了（エラーメッセージは送信しない）
	if (msg.empty() || msg.size() < 2) {
		return;
	}

	const std::string recipient = msg[0];
	const std::string content = msg[1];

	// メッセージがからの場合は何もしない
	if (content.empty()) {
		return;
	}

	if (recipient[0] == '#') {  // チャンネルに対して送信
		Channel* channel = server->FindChannelByName(recipient);
		if (!channel) {  // 一致するチャンネルが存在しない場合でもエラーを送らない
			return;
		}
		channel->SendMsgToAll(content, &client);
	} else {  // ユーザーに対して送信
		Client *recep_client = server->FindClientByNickname(recipient);
		if (!recep_client) {  // 一致するクライアントが存在しない場合でもエラーを送らない
			return;
		}
		SendMessage(recep_client->GetFd(), content, 0);
	}
	SendMessage(client.GetFd(), msg_to_c, 0);
}
