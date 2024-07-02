#include "Command.hpp"

/* PRIVMSG <target> :<message> */

bool IsCorrectFormat(std::vector<std::string> const &params, Client &client);
void SendToChannel(Client &client, const std::string &channel, const std::string &message, Server &server);
void SendToUser(Client &client, const std::string &target, const std::string &message, Server &server);
void PrintAllClients(Server &server);


/* PRIVMSGコマンド(メッセージを送信する)
 * 引数1 -> クライアント
 * 引数2 -> ニックネームとソケットファイルディスクリプタのマップ
 * 引数3 -> チャンネルのリスト */
void Command::PRIVMSG(Client &client, std::map<std::string, int> map_nick_fd, std::map<std::string, Channel*> &channels) {
	std::vector<std::string> params = message.GetParams();

	if (!IsCorrectFormat(params, client))
		return;

	const std::vector<std::string> target = SplitComma(params[0]);
	const std::string message = params[1];

	for (size_t i = 0; i < target.size(); ++i) {
		SendPrivmsg(target[i], message, client, channels, map_nick_fd);
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


/* Private Messageを送信する関数
 * 引数1 -> ターゲット
 * 引数2 -> メッセージ
 * 引数3 -> クライアント
 * 引数4 -> チャンネルのリスト */
void SendPrivmsg(const std::string &target, const std::string &message, Client &client, std::map<std::string, Channel*> &channels, std::map<std::string, int> map_nick_fd) {
	std::string const &nick = client.GetNickname();
	if (target[0] == '#') {
		const std::string channelName = &target[1];
		if (FindChannelForServer(channels, channelName) == false) {
			SendMessage(client.GetFd(), ERR_NOSUCHCHANNEL(client.GetNickname()), 0);
		}
		else {
			const Channel &channel = channels[channelName];
			const std::vector<Client *> &members = channel.GetMember();

			/* check if they are in the channel */
			if (IsInMember(members, client.GetNickname()) == false)
				SendMessage(client.GetFd(), ERR_NOTJOINCHANNEL(client.GetNickname(), channelName), 0);
			else {
				for (std::vector<Client *>::const_iterator it = members.begin(); it != members.end(); it++) {
					if ((*it)->GetNickname() != nick)
						SendMessage((*it)->GetFd(), PRIVMSG_MESSAGE(nick, client.GetUsername(), client.GetHostname(), "#" + channelName, message), 0);
				}
			}
		}
	}
	else {
		int fd;
		const std::string target_nick = target;
		if ((fd = map_nick_fd[target_nick]) == 0) {
			SendMessage(client.GetFd(), ERR_NOSUCHNICK(client.GetNickname(), target_nick), 0);
			return;
		}
		SendMessage(fd, PRIVMSG_MESSAGE(nick, client.GetUsername(), client.GetHostname(), target_nick, message), 0);
	}
}
