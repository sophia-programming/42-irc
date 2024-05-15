#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Message.hpp"

class Client {
private:
	int fd; //client file descriptor
	std::string ip_address; //client IP address
	std::string nickname; // max len 9
	std::string message_buffer; // max len 512
	Message parsed_msg;

public:
	Client();
	Client(int fd, const std::string &nick);
	~Client();

	void clearMessage();
	void parse(const std::string &message);

	int getfd() const;
	void setfd(int fd);

	void SetIPAddress(const std::string& ipaddress);
	void addMessage(const std::string &message);
	const std::string &getMessage() const;
};

#endif
