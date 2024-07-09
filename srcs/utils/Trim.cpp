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


//stringの末尾の改行を削除する関数
// 1:std::string& str ->対象のstring
void rtrim(std::string& str) {
    std::string::size_type i = str.find_last_not_of(" \t\n\r");
    if (i == std::string::npos) {
        str.clear(); // 全て空白文字の場合は空にする
    } else {
        str.erase(i + 1); // 最後の非空白文字の次を削除
    }
}