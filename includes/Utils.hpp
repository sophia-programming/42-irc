#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Channel.hpp"

class Channel;

const std::vector<std::string> SplitComma(const std::string &param);
bool FindChannelForServer(std::map<std::string, Channel*> &channels, const std::string &target);

#endif
