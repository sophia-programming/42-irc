#include "irc.hpp"

// サーバー設定の実装
int setupServer() {
	int ListenSocket = -1;
	struct addrinfo *result = NULL, hints;
	int iResult;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// ローカルアドレスとポートを解決
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed: " << iResult << std::endl;
		return 1;
	}

	//create a socket for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == -1) {
		std::cout << "Error at socket(): " << strerror(errno) << std::endl;
		freeaddrinfo(result);
		return 1;
	}

	// ここにソケット作成、バインド、リッスンのコードを追加

	freeaddrinfo(result);
	return 0;
}

int main(){
	int setupResult = setupServer();
	if (setupResult != 0) {
		std::cout << RED << "Error at setupServer(): " << setupResult << STOP << std::endl;
		return 1;
	}
	std::cout << GREEN << "server setup complete" << STOP << std::endl;
	return 0;
}
