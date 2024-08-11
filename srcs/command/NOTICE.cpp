#include "Command.hpp"
#include <sstream>

/* NOTICE <recipient> :<content> */

/* NOTICEコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::NOTICE(Client &client, Server *server, const Message &message) {
	const std::vector<std::string> msg = message.GetParams();

	// パラメータが不足している場合は処理を終了（エラーメッセージは送信しない）
	if (msg.empty() || msg.size() < 2) {
		return;
	}

	const std::string recipients = msg[0];
	const std::string content = msg[1];

	// メッセージが空の場合は何もしない
	if (content.empty()) {
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
			if (!channel) {  // 一致するチャンネルが存在しない場合でもエラーを送らない
				continue;
			}
			channel->SendMsgToAll(content, &client);
		} else {  // ユーザーに対して送信
			Client *recep_client = server->FindClientByNickname(recipient);
			if (!recep_client) {  // 一致するクライアントが存在しない場合でもエラーを送らない
				continue;
			}
			SendMessage(recep_client->GetFd(), content, 0);
		}
	}
}
