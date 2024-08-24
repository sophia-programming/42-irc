#include "Command.hpp"

/* USER <username> <hostname> <servername> <realname> */

/* USERコマンド(ユーザー情報を設定する)
 * 引数1 -> クライアント
 * 引数2 -> メッセージ */

void Command::USER(Client &client, const Message &message) {
	int fd = client.GetFd();
	std::string nickname = client.GetNickname();

	// パラメータの数を確認し、不足している場合はエラーメッセージを送信して処理を終了
	if (message.GetParams().size() != 4) {
		SendMessage(fd, ERR_NEEDMOREPARAMS(nickname, "USER"), 0);
		return; // エラーメッセージ送信後、処理を終了
	}

	// クライアントのユーザー情報を設定
	client.SetUsername(message.GetParams()[0]);
	client.SetHostname(message.GetParams()[1]);
	client.SetServername(message.GetParams()[2]);
	client.SetRealname(message.GetParams()[3]);

	// ユーザー情報が設定されたことを記録
	client.SetIsUserSet(true);

	// デバッグ目的の情報出力
	std::cout << "USER: " << client.GetUsername() << " " << client.GetHostname() << " " << client.GetServername() << " " << client.GetRealname() << std::endl;
}
