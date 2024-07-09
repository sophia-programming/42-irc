#include "Command.hpp"

bool NickSize(std::string const &nickname);
bool NickAlreadySet(std::string const &nickname, std::map<std::string, int> map_nick_fd);

/* NICKコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::NICK(Client &client, std::map<std::string, int> &map_nick_fd, std::map<std::string, Channel> &server_channels, const Message &message) {
	const int &fd = client.GetFd();

	//　引数がない場合　例）NICK
	if (message.GetParams().size() < 1) {
		SendMessage(fd, ERR_NONICKNAMEGIVEN, 0);
		return ;
	}

	std::string const OldNick = client.GetNickname();
	std::string const NewNick = message.GetParams()[0];

	// ニックネームのサイズが9文字以下かどうかを確認
	if (NickSize(NewNick) == false)
		SendMessage(fd, ERR_ERRONEUSNICKNAME(NewNick), 0);

	// ニックネームがすでに設定されているかどうかを確認
	else if (NickAlreadySet(NewNick, map_nick_fd) == true) {
		client.SetIsWelcome(false);
		SendMessage(fd, ERR_NICKNAMEINUSE(OldNick, NewNick), 0);
	}

	// 古いニックネームを削除し、新しいニックネームを追加
	else {
		map_nick_fd.erase(OldNick);
		map_nick_fd[NewNick] = fd;

		// チャンネルのオペレータリストを更新
		std::map<std::string, Channel> &channels = server_channels;
		std::map<std::string, Channel>::iterator it = channels.begin();
		for (; it != channels.end(); it++) {
			if (it->second.IsOperator(OldNick) == true) {
				it->second.RmUserFromOperator(OldNick);
				it->second.AddUserAsO(client);
			}
		}

		// ニックネームを設定
		client.SetIsNick();
		client.SetNickname(NewNick);

		// もしクライアントが認証済みの場合
		if (client.GetIsAuthenticated())
			SendMessage(fd, RPL_NICK(OldNick, NewNick), 0);

		// NICKとUSERコマンドの両方が設定されている場合, Welcomeメッセージを送信
		if (client.GetIsNick() && client.GetIsUserSet()) {
			client.SetIsWelcome(true);
			SendMessage(fd, RPL_WELCOME(NewNick), 0);
			SendMessage(fd, RPL_YOURHOST(NewNick), 0);
			SendMessage(fd, RPL_CREATED(NewNick), 0);
			SendMessage(fd, RPL_MYINFO(NewNick), 0);
		}
	}
}

/* ニックネームのサイズが9文字以下かどうかを確認する関数
 * 引数1 -> ニックネーム
 * 戻り値 -> ニックネームのサイズが9文字以下の場合はtrue, そうでない場合はfalse*/
bool NickSize(std::string const &nickname) {
	return (nickname.size() > 9) ? false : true;
}


/* ニックネームがすでに設定されているかどうかを確認する関数
 * 引数1 -> ニックネーム
 * 引数2 -> ニックネームとソケットファイルディスクリプタのマップ
 * 戻り値 -> ニックネームが設定されている場合はtrue, そうでない場合はfalse*/
bool NickAlreadySet(std::string const &nickname, std::map<std::string, int> map_nick_fd) {
	return map_nick_fd[nickname] > 0;
}
