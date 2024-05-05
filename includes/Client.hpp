#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client {
private:
	int FD; //client file descriptor
	std::string IPaddress; //client IP address

public:
	Client();
	int GetFD();
	void SeetFD(int fd);
	void setIPaddress(std::string ipaddress);
	~Client();
};

#endif
