#include "Server.hpp"

// 認証関数
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

// クライアントとの接続を処理する関数
void Server::handleConnections(int listenSocket, std::string password) {
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
