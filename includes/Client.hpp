#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Message.hpp"

class Client {
private:
	int FD; //client file descriptor
	std::string IPaddress; //client IP address
	std::string nickname; // max len 9
	std::string message_buffer; // max len 512
	Message parsed_msg;

public:
	Client();
	Client(int fd, const std::string &nick);
	~Client();

	void parse();

	int getFD() const;
	void setFD(int fd);

	void SetIPAddress(const std::string& ipaddress);
	void addMessage(const std::string &message);
	const std::string &getMessage() const;
};

#endif
