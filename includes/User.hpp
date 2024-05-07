#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"

class User {
private:
	int fd;
	std::string nickname;
	std::string message_buffer;

public:
	User();
	User(int);
	~User();
};

#endif
