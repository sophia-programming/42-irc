#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Message.hpp"

class Client {
private:
	int fd_; //client file descriptor
	bool is_authenticated_; //client authentication status
	bool is_nickname_; //client nickname status
	bool is_welcome_; //client welcome message status
	bool is_connected_; //client connection status
	std::string ip_address_; //client IP address
	std::string nickname_; // max len 9
	std::string username_;
	std::string hostname_;
	std::string servername_;
	std::string realname_;
	bool is_user_set_;
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
	bool GetIsNick() const;
	bool GetIsAuthenticated() const;
	bool GetIsWelcome() const;
	bool GetIsConnected() const;
	std::string GetIpAdress() const;
	std::string &GetMessage();
	const std::string &GetNickname() const;
	const std::string &GetUsername() const;
	const std::string &GetHostname() const;
	const std::string &GetServername() const;
	const std::string &GetRealname() const;
	bool GetIsUserSet() const;


	/* setter関数 */
	void SetFd(int fd);
	void SetNickname(const std::string &nick);
	void SetIsAuthenticated();
	void SetIsWelcome(bool iswelcome);
	void SetIsConnected();
	void SetIPAddress(const std::string& ipaddress);
	void AddMessage(const std::string &message);

	void SetIsNick();
	void SetUsername(const std::string &username);
	void SetHostname(const std::string &hostname);
	void SetServername(const std::string &servername);
	void SetRealname(const std::string &realname);
	void SetIsUserSet(bool flag);

	bool operator<(const Client& other) const;
};

#endif
