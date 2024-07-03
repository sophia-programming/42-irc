#include "Command.hpp"

/* Commaを区切り文字として文字列を分割する関数
 * 引数1 -> パラメータのvector*/
const std::vector<std::string> SplitComma(const std::string &param)
{
	std::vector<std::string> channels;

	int i = 0;
	while (param[i] && param[i] != '\r' && param[i] != '\n')
	{
		std::string channel;
		for (
				size_t j = i;
				param[j] != ',' && param[j] && param[j] != '\r' && param[j] != '\n';
				j++, i++)
			channel.append(&param[j], 1);
		channels.push_back(channel);
		i++;
	}
	return channels;
}
