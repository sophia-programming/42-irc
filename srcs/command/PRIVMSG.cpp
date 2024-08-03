#include "Command.hpp"

/* PRIVMSG <target> :<message> */

bool IsCorrectFormat(std::vector<std::string> const &params, Client &client);

/* PRIVMSGコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> ニックネームとソケットファイルディスクリプタのマップ
 * 引数3 -> チャンネルのリスト */
void Command::PRIVMSG(Client &client, Server *server, const Message &message) {
	const std::vector<std::string> msg = message.GetParams();
	std::string msg_to_c;

	if (!IsCorrectFormat(msg, client))
		return;
	const std::string recipient = msg[0];
	const std::string content = msg[1];
	if(content.empty()){ //エラー　メッセージが空
		SendMessage(client.GetFd(), ERR_NOTEXTTOSEND(client.GetNickname()), 0);
		return ;
	}
	if(recipient[0] == '#'){ //チャンネルに対して送信
		Channel* channel = server->FindChannelByName(recipient);
		if(!channel){ // エラー　一致するチャンネルが存在しない
			SendMessage(client.GetFd(), ERR_NOSUCHCHANNEL(client.GetNickname()), 0);
			return ;
		}
		channel->SendMsgToAll(content, &client);
	}
	else{ // ユーザーに対して送信
		Client *recep_client = server->FindClientByNickname(recipient);
		if(!recep_client){ // エラー　一致するクライアントが存在しない
			SendMessage(client.GetFd(), ERR_NOSUCHNICK(client.GetNickname(),recipient), 0);
			return ;
		}
		SendMessage(recep_client->GetFd(), content, 0);
	}
	SendMessage(client.GetFd(), msg_to_c, 0);
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
