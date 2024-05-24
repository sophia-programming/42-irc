#include "Server.hpp"

//__attribute__((destructor))
//static void destructor() {
//	system("leaks -q ircserv");
//}

int main(int argc, char **argv){

	// 引数の数が正しいかをチェック
	if (!validate_args(argc, argv))
		return 1;

	int port = std::atoi(argv[1]);
	std::string password = argv[2];

	Server server(port, password);
	try {
		server.ServerInit();
	}
	catch (const std::exception &e) {
		server.Shutdown();
		std::cerr << e.what() << std::endl;
		server.Cleanup(1);
	}
	std::cout << "The Server Closed" << std::endl;
}
