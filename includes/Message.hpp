#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
// #include "Utils.hpp"

class Message {
private:
	std::string prefix_;
	std::string command_;
	std::vector<std::string> params_;
	std::string original_message_;

public:
	Message();
	Message(const std::string &message);
	~Message();

	void ParsePrefix(const std::string &message, int &i);
	void ParseCommand(const std::string &message, int &i);
	void ParseParams(const std::string &message, int &i);

	/* getter関数 */
	std::string GetPrefix() const;
	std::string GetCommand() const;
	const std::vector<std::string> &GetParams() const;

	void Clear();
};

#endif
