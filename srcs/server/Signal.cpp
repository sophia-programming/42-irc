#include "Server.hpp"

bool Server::signal_ = false;
Server* server = NULL; // グローバルサーバーインスタンス

/* シグナルハンドラ */
void Server::SetupSignal() {
	signal(SIGINT, SignalHandler);
	signal(SIGQUIT, SignalHandler);
	signal(SIGPIPE, SIG_IGN);
}

/* サーバーをクリーンアップする関数
 * 引数1 -> 終了ステータス */
void Server::Cleanup(int exitStatus) {
	CloseFds();
	// SSLコンテキストの解放
	wolfSSL_CTX_free(this->ctx_); 
	// wolfSSLライブラリのクリーンアップ
	wolfSSL_Cleanup();
	std::cerr << YELLOW << "Server closed" << STOP << std::endl;
	std::exit(exitStatus);
}


/* シグナルハンドラ
 * 引数1 -> シグナル */
void Server::SignalHandler(int signal) {
	if (server) {
		std::cerr << "Signal " << signal << " received, shutting down server" << std::endl;
		server->Cleanup(signal);
	}
}
