#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>

class Message {
private:
	std::string prefix_;
	std::string command_;
	std::vector<std::string> params_;

public:
	Message();
	~Message();

	void ParsePrefix(const std::string &message, int &i);
	void ParseCommand(const std::string &message, int &i);
	void ParseParams(const std::string &message, int &i);

	std::string GetPrefix() const;
	std::string GetCommand() const;
	std::vector<std::string> GetParams() const;

	void Clear();
};

#endif
