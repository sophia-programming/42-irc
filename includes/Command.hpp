#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Message.hpp"

class Server;
// エラーメッセージ
#define ERR_UNKNOWNCOMMAND(nick, command) "421 " + nick + " " + command + " :Unknown command\r\n"
#define ERR_ERRONEUSNICKNAME(nick, nickname) "432 " + nick + " " + nickname + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, nickname) "433 " + nick + " " + nickname + " :Nickname is already in use\r\n"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters\r\n"

void PASS(Client &client, Server *server, const Message &message);
void NICK(Client &client, std::map<std::string, int> &map_nick_fd, const Message &message);
void SendMessage(int fd, const std::string &message, int flag);

#endif
