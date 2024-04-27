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

// サーバー設定の実装
int setupServer(const char *port, std::string password) {
	int ListenSocket = -1;
	struct addrinfo hints, *result = NULL;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		std::cerr << RED << "Error at getaddrinfo: " << STOP << std::endl;
		return 1;
	}

	//create a socket for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == -1) {
		std::cerr << RED << "Error at socket(): " << strerror(errno) << STOP << std::endl;
		freeaddrinfo(result);
		return 1;
	}

	//setup the TCP listening socket
	if (bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == -1) {
		std::cerr << RED << "Error at bind(): " << strerror(errno) << STOP << std::endl;
		freeaddrinfo(result);
		close(ListenSocket);
		return 1;
	}

	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == -1) {
		std::cerr << RED << "Error at listen(): " << strerror(errno) << STOP << std::endl;
		close(ListenSocket);
		return 1;
	}

	//accept a client socket
	int ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == -1) {
		std::cerr << RED << "Error at accept(): " << strerror(errno) << STOP << std::endl;
		close(ListenSocket);
		return 1;
	}

	if (!authenticate(ClientSocket, password)) {
		std::cerr << RED << "Error at authenticate(): " << strerror(errno) << STOP << std::endl;
		close(ClientSocket);
		close(ListenSocket);
		return 1;
	}

	std::cout << "Client authenticated successfully" << std::endl;
	std::cout << "Server is listening on port " << port << std::endl;
	//Here you could add more code to handle connections and use the password
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
			std::cout << "Error at recv(): " << strerror(errno) << STOP << std::endl;
			break;  // Exit the inner loop on error
		}
	}
	close(ListenSocket);
	return 0;
}
