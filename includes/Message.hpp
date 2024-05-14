#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>

class Message {
private:
	std::string prefix;
	std::string command;
	std::vector<std::string> params;

public:
	Message();
	~Message();

	void parsePrefix(const std::string &message, int &i);
	void parseCommand(const std::string &message, int &i);
	void parseParams(const std::string &message, int &i);

	std::string getPrefix() const;
	std::string getCommand() const;
	std::vector<std::string> getParams() const;
};

#endif
