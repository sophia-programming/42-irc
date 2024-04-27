#include "irc.hpp"

int authenticate(int ClientSocket, std::string expectedPassword) {
	char buffer[1024];
	int max_attempts = 3;  // 最大試行回数
	int attempts = 0;

	while (attempts < max_attempts) {
		std::string prompt = (attempts == 0 ? "Password: " : "Password incorrect, try again: ");
		std::cout << GREEN << prompt << STOP << std::endl;
		send(ClientSocket, prompt.c_str(), prompt.size() + 1, 0);

		ssize_t bytesReceived = recv(ClientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived > 0) {
			buffer[bytesReceived] = '\0';

			// Remove possible newline character
			std::string receivedPassword = buffer;
			receivedPassword.erase(std::remove(receivedPassword.begin(), receivedPassword.end(), '\n'), receivedPassword.end());

			if (expectedPassword == receivedPassword) {
				std::cout << "Password correct" << std::endl;
				std::string response = "Password correct";
				send(ClientSocket, response.c_str(), response.size() + 1, 0);
				return 1;  // Authentication successful
			} else {
				std::cout << YELLOW << "Password incorrect" << STOP << std::endl;
				attempts++;
			}
		} else if (bytesReceived == 0) {
			std::cerr << RED << "Connection closed by client" << STOP << std::endl;
			return 0;
		} else {
			std::cerr << RED << "Error at recv(): " <<  STOP << std::endl;
			return 0;
		}
	}

	std::string response = "Password incorrect, access denied.";
	send(ClientSocket, response.c_str(), response.size() + 1, 0);
	return 0;
}

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
