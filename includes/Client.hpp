#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client {
public:
	std::string nickname;
	std::string username;

	void setNickname(const std::string &nick);
	void setUsername(const std::string &user);
};

#endif
