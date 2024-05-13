#include "Server.hpp"

#include <stdlib.h>
#include <cassert>
//__attribute__((destructor))
//static void destructor() {
//	system("leaks -q ircserv");
//}


int main(int argc, char **argv){
	(void)argc;
	(void)argv;

	Server server;
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
