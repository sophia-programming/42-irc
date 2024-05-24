#include "Server.hpp"

Server::Server() {}

Server::Server(int port, const std::string &password) : port_(port), password_(password) {}

Server::~Server() {
	CloseFds();
}

/* サーバーを初期化する関数 */
void Server::ServerInit() {
	this->port_ = 6667;

	// シグナルをsetup
	SetupSignal();
	// サーバーソケットをsetup
	SetupServerSocket();
	// クライアントを受け入れる
	MakePoll(server_socket_fd_);

	std::cout << GREEN << "Server <" << server_socket_fd_ << "> is listening on port " << port_ << STOP << std::endl;

	while (true) {
		if ((poll(&fds_[0], fds_.size(), -1) == -1) && Server::signal_ == false) // wait for an event
			throw (std::runtime_error("poll() failed"));

		// 全てのファイルディスクリプタをチェック
		for (size_t i = 0; i < fds_.size(); i++) {
			// 読み込むデータがあるかどうかを確認
			if (fds_[i].revents & POLLIN) {
				// もし新しいクライアントが接続された場合
				if (fds_[i].fd == server_socket_fd_)
					AcceptNewClient();
				else
					ReceiveData(fds_[i].fd);
			}
		}
	}
	Shutdown();
}

/* Serverをシャットダウンする関数 */
void Server::Shutdown() {
	CloseFds();
}

/* クライアントからデータを受信する */
void Server::ReceiveData(int fd) {
	char buff[1024] = {0}; // buffer to received data
	memset(buff, 0, sizeof(buff)); // clear buffer

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); // receive data from client
	if (bytes <= 0) {
		std::cout << RED << "Client " << fd << " disconnected" << STOP << std::endl;
		ClearClients(fd);
		close(fd);
		return;
	} else {
		buff[bytes] = '\0';
		std::cout << YELLOW << "Client <" << fd << "> : " << buff << STOP;

		Client &user = users_[fd]; // get user from map
		user.AddMessage(std::string(buff)); // add message to user message buffer

		const std::string message = user.GetMessage(); // get message from user message buffer
		if (message.find("\r\n") != std::string::npos) {
			if (!user.IsAuthenticated()) {
				std::cout << "Client " << fd << " is not authenticated. Received password: " << message << std::endl;
				if (message == password_ + "\r\n") {
					user.Authenticate(); // authenticate user
					user.SetNickname("Client" + std::to_string(fd)); // set nickname
					std::string welcome_message = "Welcome to the chat room " + user.GetNickname() + "\n";
					SendData(fd, welcome_message, welcome_message.size());
					std::cout << GREEN << "New Client <" << fd << "> connected" << STOP << std::endl;
				} else {
					std::string error_message = "Incorrect password. Please try again\n";
					SendData(fd, error_message, error_message.size());
					user.ClearMessage(); // メッセージバッファをクリアする
					return;
				}
				user.ClearMessage();
			} else {
				std::cout << "Client " << fd << " is authenticated. Processing message: " << message << std::endl;
				user.Parse(message); // 認証済みの場合のみメッセージをパースする
			}
		}
	}
}


/* クライアントにデータを送信する関数
 * 引数1 -> クライアントのソケットファイルディスクリプタ
 * 引数2 -> 送信するメッセージ
 * 引数3 -> 送信するメッセージのサイズ*/
void Server::SendData(int fd, std::string message, int size) {
	send(fd, message.c_str(), size, 0);
}


/* クライアントをクリアする関数
 * 引数1 -> クリアするクライアントのソケットファイルディスクリプタ*/
void Server::ClearClients(int fd) {
	for (size_t i = 0; i < fds_.size(); i++) { // clear from the pollfd
		if (fds_[i].fd == fd) {
			fds_.erase(fds_.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < connected_clients.size(); i++) { // clear clients from  the vector
		if (connected_clients[i].GetFd() == fd) {
			connected_clients.erase(connected_clients.begin() + i);
			break;
		}
	}
}


/*　全てのfdをcloseする関数　*/
void Server::CloseFds() {
	for (size_t i = 0; i < connected_clients.size(); i++) { // close all clients
		std::cout << RED << "Client " << connected_clients[i].GetFd() << " disconnected" << STOP << std::endl;
		close(connected_clients[i].GetFd());
	}
	if (server_socket_fd_ != -1) { // close server socket
		std::cout << RED << "Server socket " << server_socket_fd_ << " Disconnected" << STOP << std::endl;
		close(server_socket_fd_);
		// socketが閉じられたことを記録
		server_socket_fd_ = -1;
	}
}


/*========== Socket通信のための関数群 ==========*/


/* Clientの初期設定
 1: 引数(socketfd) -> クライアントのソケットファイルディスクリプタ*/
void Server::SetupClient(int socketfd) {
	const std::string nick = "unknown" + std::to_string(socketfd); // set default nickname
	Client user(socketfd, nick); // create new user
	users_[socketfd] = user; // add user to map
}


/* 新しいクライアントを受け入れる */
void Server::AcceptNewClient() {
	struct sockaddr_in clientAddress;
	socklen_t len = sizeof(clientAddress);

	// accept new client
	int incomingfd = accept(server_socket_fd_, (struct sockaddr *)&clientAddress, &len);
	if (incomingfd == -1){
		std::cout << RED << "accept() failed" << STOP << std::endl;
		return ;
	}

	// set client socket to non-blocking
	if (fcntl(incomingfd, F_SETFL, O_NONBLOCK) == -1) {
		std::cout << RED << "fcntl() failed" << STOP << std::endl;
		close(incomingfd);
		return ;
	}

	// initialize client
	SetupClient(incomingfd);

	Client &client = users_[incomingfd];
	// set client IP address
	client.SetIPAddress(inet_ntoa(clientAddress.sin_addr));
	// add client to vector
	connected_clients.push_back(client);

	// call MakePoll with the new client's fd
	MakePoll(incomingfd);
	std::cout << GREEN << "New client <" << incomingfd << "> connected" << STOP << std::endl;
}


/* pollシステムコールを使用するためのにpollfd構造体を作成する
 1: 引数(socketfd) -> クライアントのソケットファイルディスクリプタ*/
void Server::MakePoll(int socketfd) {
	struct pollfd NewPoll;

	// pollfd file descriptorをsocketに設定
	NewPoll.fd = socketfd;

	// pollfd eventsをPOLLINに設定
	NewPoll.events = POLLIN;

	// pollfd reventsを0に設定
	NewPoll.revents = 0;

	// pollfdをvectorに追加
	fds_.push_back(NewPoll);
}


/* サーバーソケットを作成する */
void Server::SetupServerSocket() {
	/*=== create server socket ===*/
	server_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket_fd_ == -1)
		throw (std::runtime_error("Server socket creation failed"));

	/*=== set server address ===*/
	// create server address
	struct sockaddr_in address;
	// set address family to IPv4
	address.sin_family = AF_INET;
	// set address to any interface
	address.sin_addr.s_addr = INADDR_ANY;
	// set port
	address.sin_port = htons(this->port_);

	/*=== set socket options ===*/
	int en = 1;
	// set socket options to reuse address
	if (setsockopt(server_socket_fd_, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1){
		close(server_socket_fd_);
		throw (std::runtime_error("Server socket options failed"));
	}

	// set socket to non-blocking
	if (fcntl(server_socket_fd_, F_SETFL, O_NONBLOCK) == -1) {
		close(server_socket_fd_);
		throw (std::runtime_error("Server socket fcntl failed"));
	}

	/*=== bind server socket to address ===*/
	if (bind(server_socket_fd_, (struct sockaddr *)&address, sizeof(address)) == -1){
		close(server_socket_fd_);
		throw (std::runtime_error("Server socket bind failed"));
	}

	/*=== listen on server socket ===*/
	if (listen(server_socket_fd_, SOMAXCONN) == -1){
		close(server_socket_fd_);
		throw(std::runtime_error("Server socket listen failed"));
	}
}

/* getter関数 */
std::map<int, Client> Server::GetUsers() {
	return users_;
}

int Server::GetServerSocketFd() const {
	return server_socket_fd_;
}

// 既存のチャンネルか確認する
// 1: std::string& name -> 確認したいチャンネル名
//bool Server::IsChannel(std::string& name) {
//	std::map<std::string, Channel>::iterator iter = this->channel_list_.find(name);
//	if(iter != this->channel_list_.end()){
//		return true;
//	}
//	return false;
//}

// チャンネル名から検索してchannelオブジェクトを取得する
// 1:std::string& name -> 取得したいチャンネル名
//Channel Server::GetChannel(std::string& name)
//{
//	std::map<std::string, Channel>::iterator iter = this->channel_list_.find(name);
//	if(iter != this->channel_list_.end()){
//		return iter->second;
//	}
//	throw std::exception();
//}

//チャンネルを作成してリストに登録する
// 1:std::string& name　-> 作成したいチャンネル名
//void Server::CreateChannel(std::string& name)
//{
//	if(name[0] != '#'){
//		// error plese create #channel name
//		return;
//	}
//	this->channel_list_.emplace(name, Channel(name));
//}
