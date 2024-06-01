#include "Command.hpp"

bool NickSize(std::string const &nickname);
bool NickAlreadySet(std::string const &nickname, std::map<std::string, int> const &map_nick_fd);

/* NICKコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void NICK(Client &client, std::map<std::string, int> &map_nick_fd, const Message &message) {
	int fd = client.GetFd();

	//　引数がない場合　例）NICK
	if (message.GetParams().size() < 1){
		SendMessage(fd, std::string(YELLOW) + ERR_NEEDMOREPARAMS(client.GetNickname(), "NICK") + std::string(STOP), 0);
		return ;
	}

	std::string const OldNick = client.GetNickname();
	std::string const NewNick = message.GetParams()[0];

	// ニックネームのサイズが9文字以下かどうかを確認
	if (NickSize(NewNick) == false)
		SendMessage(fd, ERR_ERRONEUSNICKNAME(OldNick, NewNick), 0);

	// ニックネームがすでに設定されているかどうかを確認
	else if (NickAlreadySet(NewNick, map_nick_fd) == true)
		SendMessage(fd, ERR_NICKNAMEINUSE(OldNick, NewNick), 0);

	// ニックネームを変更
	else {
		map_nick_fd.erase(OldNick);
		map_nick_fd[NewNick] = fd;
		client.SetNickname(NewNick);
	}
}


/* ニックネームのサイズが9文字以下かどうかを確認する関数
 * 引数1 -> ニックネーム
 * 戻り値 -> ニックネームのサイズが9文字以下の場合はtrue, そうでない場合はfalse*/
bool NickSize(std::string const &nickname){
	return (nickname.size() > 9) ? false : true;
}


/* ニックネームがすでに設定されているかどうかを確認する関数
 * 引数1 -> ニックネーム
 * 引数2 -> ニックネームとソケットファイルディスクリプタのマップ
 * 戻り値 -> ニックネームが設定されている場合はtrue, そうでない場合はfalse*/
bool NickAlreadySet(std::string const &nickname, std::map<std::string, int> const &map_nick_fd){
	return (map_nick_fd.find(nickname) != map_nick_fd.end());
}
