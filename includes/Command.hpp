#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Message.hpp"

class Server;
// エラーメッセージ
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters\r\n"

void PASS(Client &client, Server *server, const Message &message);
void SendMessage(int fd, const std::string &message, int flag);

#endif
