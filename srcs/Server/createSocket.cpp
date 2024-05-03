#include "Irc.hpp"

//構造体の初期化: memset()関数を使って構造体をゼロクリアする
int initializeServer(const char* port, int &listenSocket) {
	struct addrinfo hints, *result = NULL;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//addrinfo 構造体に関連する hints を設定
	//AF_INET : IPv4インターネットプロトコルを示す
	//SOCK_STREAM : TCPソケットタイプを示す
	//IPPROTO_TCP : TCPプロトコルを示す
	//AI_PASSIVE : バインドするためのアドレスを返す（NULLが渡された場合、自動的にローカルアドレスを選択）

	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		std::cerr << RED << "Error at getaddrinfo: " << STOP << std::endl;
		return -1;
	}

	//socket()関数を使ってソケットを作成
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == -1) {
		std::cerr << RED << "Error at socket(): " << strerror(errno) << STOP << std::endl;
		freeaddrinfo(result);
		return -1;
	}

	//bind()関数を使ってソケットにアドレスをバインド
	if (bind(listenSocket, result->ai_addr, result->ai_addrlen) == -1) {
		std::cerr << RED << "Error at bind(): " << strerror(errno) << STOP << std::endl;
		freeaddrinfo(result);
		close(listenSocket);
		return -1;
	}

	freeaddrinfo(result);

	//listen()関数を使って接続待ちの状態にする
	if (listen(listenSocket, SOMAXCONN) == -1) {
		std::cerr << RED << "Error at listen(): " << strerror(errno) << STOP << std::endl;
		close(listenSocket);
		return -1;
	}

	return 0;
}
