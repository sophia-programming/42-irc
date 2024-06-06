#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Message.hpp"

class Server;

// Welcomeメッセージ(001 ~ 004)
#define RPL_WELCOME(nick) ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Chat Network " + nick + "\r\n"
#define RPL_YOURHOST(nick) ":ft_irc 002 " + nick + " :Your host is ft_irc, running version 1.0\r\n"
#define RPL_CREATED(nick) ":ft_irc 003 " + nick + " :This server was created in C++ [Sunday, June 16, 2024]\r\n"
#define RPL_MYINFO(nick) ":ft_irc 004 " + nick + " :FT_IRC 1.0 contributes: oaoba yushimom stakimot \r\n"

// ニックネーム変更メッセージ
#define RPL_NICK(oldnick, newnick) ":" + oldnick + " NICK :" + newnick + "\r\n"

// エラーメッセージ
#define ERR_UNKNOWNCOMMAND(nick, command) ":ft_irc 421 " + nick + " " + command + " :Unknown command\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ":ft_irc 432 " + nick + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, nickname) ":ft_irc 433 " + nick + " " + nickname + " :Nickname is already in use\r\n"
#define ERR_NOTREGISTERED(nick) ":ft_irc 451 " + nick + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) ":ft_irc 461 " + nick + " " + command + " :Not enough parameters\r\n"

void PASS(Client &client, Server *server, const Message &message);
void NICK(Client &client, std::map<std::string, int> &map_nick_fd, const Message &message);
void SendMessage(int fd, const std::string &message, int flag);
void SendWelcomeMessage(const Client &client);

#endif
