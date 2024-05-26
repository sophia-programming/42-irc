#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Message.hpp"

// エラーメッセージ
#define ERR_PARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters\r\n"

void PASS(Client &client, const std::string &server_password, const Message &message);
void SendMessage(int fd, const std::string &message, int flag);

#endif
