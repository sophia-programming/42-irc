#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Message.hpp"

class Client {
private:
	int fd_; //client file descriptor
	std::string ip_address_; //client IP address
	std::string nickname_; // max len 9
	std::string message_buffer_; // max len 512
	Message parsed_msg_;

public:
	Client();
	Client(int fd_, const std::string &nick);
	~Client();

	void ClearMessage();
	void Parse(const std::string &message);

	int GetFd() const;
	void SetFd(int fd);

	std::string GetNickname() const;
	std::string GetIpAdress() const;

	void SetIPAddress(const std::string& ipaddress);
	void AddMessage(const std::string &message);
	const std::string &GetMessage() const;
};

#endif
