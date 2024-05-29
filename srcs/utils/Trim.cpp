#include "Server.hpp"

/* コマンドの前後の空白を取り除く
	 * find_last_not_of() -> 末尾の空白を削除
	 * find_first_not_of() -> 先頭の空白を削除*/
std::string Trim(const std::string &str){
	std::string trimmed = str;
	trimmed.erase(trimmed.find_last_not_of(" \n\r\t") + 1);
	trimmed.erase(0, trimmed.find_first_not_of(" \n\r\t"));
	return trimmed;
}
