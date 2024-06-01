#include "Server.hpp"

Server::Server() {}

Server::Server(int port, const std::string &password) : port_(port), password_(password) {}

Server::~Server() {
	CloseFds();
}


/* Serverを初期化する関数
 * 引数　1 -> ポート番号*/
void Server::ServerInit(int port) {
	this->port_ = port;

	// シグナルをsetup
	SetupSignal();
	// サーバーソケットをsetup
	SetupServerSocket();
	// クライアントを受け入れる
	MakePoll(server_socket_fd_);
}


/* サーバーを開始する関数 */
void Server::ServerStart() {
	while (true) {
		// poll()でデータを待機
		if ((poll(&fds_[0], fds_.size(), TIMEOUT) == -1) && Server::signal_ == false)
			throw (std::runtime_error("poll() failed"));

		// 全てのファイルディスクリプタをチェック
		for (size_t i = 0; i < fds_.size(); i++) {
			// 読み込むデータがあるかどうかを確認
			if (fds_[i].revents & POLLIN) {
				// もし新しいクライアントが接続された場合
				if (fds_[i].fd == server_socket_fd_)
					AcceptNewClient();
				else
					ChatFlow(fds_[i].fd);
			}
		}
	}
	CloseFds();
}


/* クライアンドからのメッセージを受信し、処理する関数
 * 1. クライアントからデータを受信
 * 2. 受信したデータをクライアントのメッセージバッファに追加 AddMessage()
 * 3. メッセージバッファ内のメッセージをパース Parse()
 * 4. コマンドを処理 ExecuteCommand()
 * 引数1 -> クライアントのソケットファイルディスクリプタ */
void Server::ChatFlow(int fd) {
	// 受け取ったデータを格納するバッファ
	char received_data[1024] = {0};

	// データを受信
	ssize_t bytes = recv(fd, received_data, sizeof(received_data), 0);
	if (bytes <= 0) {
		std::cout << RED << "Client " << fd << " disconnected" << STOP << std::endl;
		ClearClients(fd);
		close(fd);
		return;
	}

	// マップからクライアントを取得
	Client &user = users_[fd];
	// 受け取ったデータをクライアントのメッセージバッファに追加
	user.AddMessage(std::string(received_data));
	//　クライアントのメッセージバッファを取得
	std::string &message_buffer = user.GetMessage(); // ここをstd::string &に変更して直接操作する

	size_t pos = 0;
	while ((pos = message_buffer.find_first_of("\r\n")) != std::string::npos) {
		std::string parsed_message;

		// \r\n, \r, \n のいずれかの終端文字に対応
		if (message_buffer[pos] == '\r' && pos + 1 < message_buffer.size() && message_buffer[pos + 1] == '\n') {
			// \r\n
			parsed_message = message_buffer.substr(0, pos + 2);
			message_buffer.erase(0, pos + 2);
		} else {
			// \r または \n
			parsed_message = message_buffer.substr(0, pos + 1);
			message_buffer.erase(0, pos + 1);
		}

		// 受け取ったコマンドをパース
		Message parsed_msg(parsed_message);
		// コマンドを処理
		ExecuteCommand(fd, parsed_msg);
	}
}


/* Commandを処理する関数
 * 引数1 -> クライアントのソケットファイルディスクリプタ*/
void Server::ExecuteCommand(int fd, const Message &message) {
	Client &client = users_[fd];
	std::string cmd = message.GetCommand();
	const std::vector<std::string> &params = message.GetParams();

	/* コマンドの前後の空白を取り除く */
	cmd = Trim(cmd);

	if (cmd == "PASS")
		Command::PASS(client, this, message);
	else
		SendMessage(fd, std::string(YELLOW) + "Invalid command. Please enter a <PASS password>\r\n" + std::string(STOP), 0);
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
		// clear from the pollfd
		for (size_t i = 0; i < fds_.size(); i++) {
			if (fds_[i].fd == fd) {
				fds_.erase(fds_.begin() + i);
				break;
			}
		}
		// clear clients from the map
		for (size_t i = 0; i < connected_clients.size(); i++) {
			if (connected_clients[i].GetFd() == fd) {
				connected_clients.erase(connected_clients.begin() + i);
				break;
			}
		}
	}


/*　全てのfdをcloseする関数　*/
void Server::CloseFds() {
	// close all clients
	for (std::map<int, Client>::iterator iter = users_.begin(); iter != users_.end(); iter++) {
		std::cout << "close: " << iter->second.GetNickname() << " [" << iter->second.GetFd() << "]" << std::endl;
		close(iter->first);
	}
	// close server socket
	if (server_socket_fd_ != -1) {
		std::cout << RED << "Server socket " << server_socket_fd_ << " Disconnected" << STOP << std::endl;
		close(server_socket_fd_);
		// socketが閉じられたことを記録
		server_socket_fd_ = -1;
	}
}


/* passwordを検証する関数
 * 引数1 -> 入力されたパスワード
 * 戻り値 -> パスワードが正しい場合はtrue、それ以外はfalse*/
bool Server::CheckPassword(const std::string &password) const {
	return password == this->password_;
}


/*========== Socket通信のための関数群 ==========*/


/* Clientの初期設定
 1: 引数(socketfd) -> クライアントのソケットファイルディスクリプタ*/
	void Server::SetupClient(int socketfd) {
		// set client nickname
		const std::string nick = "unknown" + std::to_string(socketfd);
		// create new user
		Client user(socketfd, nick);
		// add user to map
		users_[socketfd] = user;
	}


/* 新しいクライアントを受け入れる */
	void Server::AcceptNewClient() {
		struct sockaddr_in clientAddress;
		socklen_t len = sizeof(clientAddress);

		// accept new client
		int incomingfd = accept(server_socket_fd_, (struct sockaddr *) &clientAddress, &len);
		if (incomingfd == -1) {
			std::cout << RED << "accept() failed" << STOP << std::endl;
			return;
		}

		// set client socket to non-blocking
		if (fcntl(incomingfd, F_SETFL, O_NONBLOCK) == -1) {
			std::cout << RED << "fcntl() failed" << STOP << std::endl;
			close(incomingfd);
			return;
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
		if (setsockopt(server_socket_fd_, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) == -1) {
			close(server_socket_fd_);
			throw (std::runtime_error("Server socket options failed"));
		}

		// set socket to non-blocking
		if (fcntl(server_socket_fd_, F_SETFL, O_NONBLOCK) == -1) {
			close(server_socket_fd_);
			throw (std::runtime_error("Server socket fcntl failed"));
		}

		/*=== bind server socket to address ===*/
		if (bind(server_socket_fd_, (struct sockaddr *) &address, sizeof(address)) == -1) {
			close(server_socket_fd_);
			throw (std::runtime_error("Server socket bind failed"));
		}

		/*=== listen on server socket ===*/
		if (listen(server_socket_fd_, SOMAXCONN) == -1) {
			close(server_socket_fd_);
			throw (std::runtime_error("Server socket listen failed"));
		}
	}

/* getter関数 */
std::string Server::GetPassword() const {
	return password_;
}

std::map<int, Client> Server::GetUsers() {
	return users_;
}

int Server::GetServerSocketFd() const {
	return server_socket_fd_;
}


/* setter関数 */
std::string Server::SetPassword(const std::string &password) {
	this->password_ = password;
}

// 既存のチャンネルか確認する
// 1: std::string& name -> 確認したいチャンネル名
bool Server::ChannelExist(const std::string& name) {
	std::map<std::string, Channel>::iterator iter = this->channel_list_.find(name);
	if(iter != this->channel_list_.end()){
		return true;
	}
	return false;
}

// チャンネル名から検索してchannelオブジェクトを取得する
// 1:std::string& name -> 取得したいチャンネル名
Channel Server::GetChannel(const std::string& name)
{
	std::map<std::string, Channel>::iterator iter = this->channel_list_.find(name);
	if(iter != this->channel_list_.end()){
		return iter->second;
	}
	throw std::exception();
}

//チャンネルを作成してリストに登録する
// 1:std::string& name　-> 作成したいチャンネル名
void Server::CreateChannel(const std::string& name)
{
	if(name[0] != '#'){
		// error plese create #channel name
		return;
	}
	this->channel_list_.insert(std::pair{name, Channel(name)});
}
