#include "Server.hpp"

bool Server::signal_ = false;
Server* server = nullptr; // グローバルサーバーインスタンス

/* シグナルハンドラ */
void Server::SetupSignal() {
	signal(SIGINT, SignalHandler);
	signal(SIGQUIT, SignalHandler);
	signal(SIGPIPE, SIG_IGN);
}

/* サーバーをクリーンアップする関数
 * 引数1 -> 終了ステータス */
void Server::Cleanup(int exitStatus) {
	if (server) {
		std::map<int, Client>::iterator iter = server->GetUsers().begin();

		for(; iter != server->GetUsers().end(); iter++) {
			std::cout << "close: " << iter->second.GetNickname() << " [" << iter->second.GetFd() << "]" << std::endl;
			close(iter->first);
		}
		close(server->GetServerSocketFd());
		std::cerr << "Server closed" << std::endl;
		std::exit(exitStatus);
	}
}

/* シグナルハンドラ
 * 引数1 -> シグナル */
void Server::SignalHandler(int signal) {
	if (server) {
		std::cerr << "Signal " << signal << " received, shutting down server" << std::endl;
		server->Cleanup(signal);
	}
}
