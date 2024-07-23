#include "Command.hpp"

/* QUIT <message> */

/* QUITコマンド(クライアントを切断する)
 * 引数1 -> クライアント
 * 引数2 -> メッセージ */

void QUIT(Client &client, Server *server, std::vector<struct pollfd> &pollfds,
		std::map<int, Client> &users, std::map<std::string, int> &nick_to_fd,
		const std::vector<std::string> &params, const Message &message) {
	const std::string nick = client.GetNickname();
	const int &ClientFd = client.GetFd();

	std::string QuitMessage = "leaving";

	if (!params.empty())
		QuitMessage = message.GetParams()[0];

	if (!server->GetChannels().empty())
	{
		std::map<std::string, Channel*> &channels = server->GetChannels();
		for (
				std::map<std::string, Channel*>::iterator it = channels.begin();
				it != channels.end();
				it++
				) {
			const std::vector<Client *> &members = it->second->GetMember();
			for (
					std::vector<Client *>::const_iterator mem_it = members.begin();
					mem_it != members.end();
					mem_it++
					) {
				const int memFd = (*mem_it)->GetFd();
				if (memFd == ClientFd)
					it->second->RmUser(&client);
				else
					SendMessage(memFd, QUIT_MESSAGE(nick, client.GetUsername(), client.GetHostname(), QuitMessage), 0);
			}
		}
	}
}
