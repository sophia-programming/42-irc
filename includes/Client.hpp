#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Message.hpp"

class Client {
private:
	int fd_; //client file descriptor
	bool is_authenticated_; //client authentication status
	bool is_nickname_; //client nickname status
	bool is_connected_;
	std::string ip_address_; //client IP address
	std::string nickname_; // max len 9
	std::string message_buffer_; // max len 512
	Message parsed_msg_;


public:
	Client();
	Client(int fd_, const std::string &nick);
	~Client();

	void Parse(const std::string &message);
	void ClearMessage();
	void Debug_parser();

	/* getter関数 */
	int GetFd() const;
	std::string GetNickname() const;
	std::string GetIpAdress() const;
	std::string &GetMessage() ;


	/* setter関数 */
	void SetFd(int fd);
	void SetNickname(const std::string &nick);
	void SetIPAddress(const std::string& ipaddress);
	void AddMessage(const std::string &message);

	void Authenticate();
	bool IsAuthenticated() const;

	// ===== mapに使うには比較可能な型である必要があるのでそのためのオーバーロード =====
	bool operator<(const Client& other) const {
        return this->nickname_ < other.GetNickname();  // 例として username を比較基準にしています
	}
};

#endif
