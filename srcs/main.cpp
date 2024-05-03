#include "Server.hpp"

#include <stdlib.h>
//__attribute__((destructor))
//static void destructor() {
//	system("leaks -q ircserv");
//}

int main(int argc, char **argv){
	if (argc != 3){
		std::cout << GREEN << "Usage: " << argv[0] << " <port> <password>" << STOP << std::endl;
		return 1;
	}

	// サーバーのインスタンスを作成
	Server myServer;

	// サーバーの初期化
	int listenSocket;
	if (myServer.initializeServer(argv[1], listenSocket) != 0) {
		std::cerr << "Failed to initialize server." << std::endl;
		return 1;
	}

	// クライアントとの接続を処理
	myServer.handleConnections(listenSocket, argv[2]);
	close(listenSocket);
	return 0;
}
