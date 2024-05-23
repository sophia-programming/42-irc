#include "Server.hpp"

// ./ircserv [port] [password]の形式かをチェックする関数
bool validate_args(int argc, char **argv) {

	// 引数が3つでない場合はエラー(./ircserv [port] [password]の形式)
	if (argc != 3) {
		std::cerr << RED << "Usage: ./ircserv [port] [password]" << STOP << std::endl;
		return false;
	}

	int port = std::atoi(argv[1]);

	// port番号の有効な値の範囲は1から65535
	if (port <= 0)	{
		std::cerr << RED << "Invalid port number" << STOP << std::endl;
		return false;
	}

	// port番号が65535以上の場合はエラー
	if (65535 < port) {
		std::cerr << RED << "Port number must be less than 65535" << STOP << std::endl;
		return false;
	}
	return true;
}
