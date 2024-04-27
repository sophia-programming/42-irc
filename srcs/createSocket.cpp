#include "irc.hpp"

// サーバー設定の実装
int setupServer() {
	int ListenSocket = -1;
	struct addrinfo *result = NULL, hints;
	int iResult;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// ローカルアドレスとポートを解決
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed: " << iResult << std::endl;
		return 1;
	}

	//create a socket for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == -1) {
		std::cout << "Error at socket(): " << strerror(errno) << std::endl;
		freeaddrinfo(result);
		return 1;
	}

	//setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == -1) {
		std::cout << "Error at bind(): " << strerror(errno) << std::endl;
		freeaddrinfo(result);
		close(ListenSocket);
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == -1) {
		std::cout << "Error at listen(): " << strerror(errno) << std::endl;
		freeaddrinfo(result);
		close(ListenSocket);
		return 1;
	}

	freeaddrinfo(result);
	std::cout << "Server is listening on port " << DEFAULT_PORT << std::endl;

	while (true) {
		int ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == -1) {
			std::cout << "Error at accept(): " << strerror(errno) << std::endl;
			close(ListenSocket);
			return 1;
		}

		std::cout << GREEN << "Client connected" << STOP << std::endl;

		// Loop to receive messages continuously until the client disconnects
		while (true) {
			char buffer[1024];
			ssize_t bytesReceived = recv(ClientSocket, buffer, sizeof(buffer), 0);
			if (bytesReceived > 0) {
				std::cout << YELLOW << "Received message: " << STOP << std::string(buffer, bytesReceived) << std::endl;
				std::string response = "Message received: " + std::string(buffer, bytesReceived);
				send(ClientSocket, response.c_str(), response.size() + 1, 0);
			} else if (bytesReceived == 0) {
				std::cout << "Client disconnected" << std::endl;
				break;  // Exit the inner loop and wait for a new connection
			} else {
				std::cout << "Error at recv(): " << strerror(errno) << std::endl;
				break;  // Exit the inner loop on error
			}
		}

		close(ClientSocket);
	}

	close(ListenSocket);
	return 0;
}

int main(){
	int setupResult = setupServer();
	if (setupResult != 0) {
		std::cout << RED << "Error at setupServer(): " << setupResult << STOP << std::endl;
		return 1;
	}
	std::cout << GREEN << "server setup complete" << STOP << std::endl;
	return 0;
}
