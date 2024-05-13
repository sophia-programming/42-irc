#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"
#include "Message.hpp"

class User {
private:
	int fd;
	std::string nickname; //max len 9
	std::string message_buffer; //max len 512
	Message parsed_msg;

public:
	User();
	User(int fd, const std::string &nick);
	~User();

	void parse();

	void addMessage(const std::string &message);
	const std::string &getMessage() const;
};

#endif
