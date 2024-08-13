#include "Utils.hpp"

long long int string_to_llint(const std::string& str) {
    std::istringstream iss(str);
    long long int num;
    iss >> num;

    // 変換に失敗した場合は例外をスローするか、適切なエラーハンドリングを行う必要があります。
    if (iss.fail()) {
        throw std::invalid_argument("Invalid input string for conversion to integer");
    }

    return num;
}