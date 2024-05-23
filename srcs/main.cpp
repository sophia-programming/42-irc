#include "Server.hpp"

#include "stdlib.h"
//__attribute__((destructor))
//static void destructor() {
//	system("leaks -q ircserv");
//}

int main(int argc, char **argv){

	// 引数の数が正しいかをチェック
	if (!validate_args(argc, argv))
		return 1;

	Server server(argv[1], argv[2]);
	std::cout << YELLOW << "====== Server ======" << STOP << std::endl;
	try {
//		signal(SIGINT, Server::SignalHandler); // catch the signal(ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); // catch the signal(ctrl + \)
		server.ServerInit(); // initialize server
	}
	catch (const std::exception &e) {
		server.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed" << std::endl;
}
