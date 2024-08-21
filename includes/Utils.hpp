#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "Channel.hpp"

class Channel;
const std::vector<std::string> SplitComma(const std::string &param);
bool FindChannelForServer(std::map<std::string, Channel*> &channels, const std::string &target);
// void rtrim(std::string& str);
long int string_to_lint(const std::string& str);

template <typename T>
std::string change_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif
