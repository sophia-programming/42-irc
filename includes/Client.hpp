#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client {
private:
	int FD; //client file descriptor
	std::string IPaddress; //client IP address

public:
	Client();
	int GetFD();
	void SetFD(int fd);
	void SetIPAddress(std::string ipaddress);
	~Client();
};

#endif
