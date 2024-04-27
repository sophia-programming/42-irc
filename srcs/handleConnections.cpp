#include "irc.hpp"

void handleConnections(int listenSocket, std::string password) {
	while (true) {
		int clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == -1) {
			std::cerr << RED << "Error at accept(): " << strerror(errno) << STOP << std::endl;
			continue;
		}

		if (!authenticate(clientSocket, password)) {
			std::cerr << RED << "Error at authenticate(): " << strerror(errno) << STOP << std::endl;
			close(clientSocket);
			continue;
		}

		std::cout << "Client authenticated successfully." << std::endl;

		// Handle client messages
		char buffer[1024];
		while (true) {
			ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytesReceived > 0) {
				std::cout << YELLOW << "Received message: " << STOP << std::string(buffer, bytesReceived) << std::endl;
				std::string response = "Message received: " + std::string(buffer, bytesReceived);
				send(clientSocket, response.c_str(), response.size() + 1, 0);
			} else if (bytesReceived == 0) {
				std::cout << "Client disconnected." << std::endl;
				break;
			} else {
				std::cerr << "Error at recv(): " << strerror(errno) << STOP << std::endl;
				break;
			}
		}
		close(clientSocket);
	}
}
