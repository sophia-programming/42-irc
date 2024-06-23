#include "Command.hpp"

/* PRIVMSG <target> :<message> */

bool IsCorrectFormat(std::vector<std::string> const &params, Client &client);
void SendToChannel(Client &client, const std::string &channel, const std::string &message, Server &server);
void SendToUser(Client &client, const std::string &target, const std::string &message, Server &server);
void PrintAllClients(Server &server);


/* PRIVMSGコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> パラメータのvector */
void Command::PRIVMSG(Client &client, const std::vector<std::string> &params, Server &server) {
	if (!IsCorrectFormat(params, client))
		return;

	std::string target = params[0];
	std::string message = params[1];
	for (size_t i = 2; i < params.size(); ++i) {
		message += " " + params[i];
	}
	// メッセージ先頭のコロンを削除
	if (message[0] == ':')
		message = message.substr(1);

	// channelは#から始まる
	if (target[0] == '#')
		SendToChannel(client, target, message, server);
	else
		SendToUser(client, target, message, server);
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

/* メンバーに含まれているかどうかを確認する関数
 * 引数1 -> メンバーのvector
 * 引数2 -> 名前 */
bool IsInMember(std::vector<Client *> const &members, std::string const &name) {
	for (std::vector<Client *>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if ((*it)->GetNickname() == name)
			return true;
	}
	return false;
}

/* チャンネルへのメッセージ送信
 * 引数1 -> クライアント
 * 引数2 -> チャンネル名
 * 引数3 -> メッセージ */
void SendToChannel(Client &client, const std::string &channel, const std::string &message, Server &server) {
	int fd = client.GetFd();

	//channel名からチャンネルオブジェクトを取得
	Channel* targetChannel = server.FindChannelByName(channel);

	//チャンネルオブジェクトが存在する場合、メッセージを送信
	if (targetChannel)
		SendMessage(fd, PRIVMSG_MESSAGE(channel, message), 0);
	else
		SendMessage(fd, ERR_NOSUCHCHANNEL(client.GetNickname()), 0);
}


/* ユーザーへのメッセージ送信
 * 引数1 -> クライアント
 * 引数2 -> ユーザー名
 * 引数3 -> メッセージ
 * 引数4 -> サーバー */
void SendToUser(Client &client, const std::string &target, const std::string &message, Server &server) {
	int fd = client.GetFd();
	Client* targetClient = server.FindClientByNickname(target, client);
	//nicknameからクライアントオブジェクトを取得
	std::cout << "target = " << target << std::endl;
	std::cout << "message = " << message << std::endl;

	std::cout << "targetClient: " << targetClient << std::endl;

	//クライアントオブジェクトが存在する場合、メッセージを送信
	if (targetClient)
		SendMessage(targetClient->GetFd(), PRIVMSG_MESSAGE(client.GetNickname(), message), 0);
	else
		SendMessage(fd, ERR_NOSUCHNICK(client.GetNickname()), 0);
}
