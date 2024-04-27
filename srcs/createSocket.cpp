#include "irc.hpp"

int initializeServer(const char* port, int &listenSocket) {
	struct addrinfo hints, *result = NULL;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		std::cerr << RED << "Error at getaddrinfo: " << STOP << std::endl;
		return -1;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == -1) {
		std::cerr << RED << "Error at socket(): " << strerror(errno) << STOP << std::endl;
		freeaddrinfo(result);
		return -1;
	}

	if (bind(listenSocket, result->ai_addr, result->ai_addrlen) == -1) {
		std::cerr << RED << "Error at bind(): " << strerror(errno) << STOP << std::endl;
		freeaddrinfo(result);
		close(listenSocket);
		return -1;
	}

	freeaddrinfo(result);

	if (listen(listenSocket, SOMAXCONN) == -1) {
		std::cerr << RED << "Error at listen(): " << strerror(errno) << STOP << std::endl;
		close(listenSocket);
		return -1;
	}

	return 0;
}
