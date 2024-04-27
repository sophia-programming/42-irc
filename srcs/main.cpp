#include "irc.hpp"

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

	const char *port = argv[1];
	std::string password = argv[2];

	if (setupServer(port, password) != 0) {
		std::cerr << RED << "Error setting up server" << STOP <<std::endl;
		return 1;
	}
	return 0;
}
