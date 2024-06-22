#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Client.hpp"

class Server;
class Client;
class Message;

// Welcomeメッセージ(001 ~ 004)
#define RPL_WELCOME(nick) ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Chat Network \r\n"
#define RPL_YOURHOST(nick) ":ft_irc 002 " + nick + " :Your host is ft_irc, running version 1.0\r\n"
#define RPL_CREATED(nick) ":ft_irc 003 " + nick + " :This server was created in C++ [Sunday, June 16, 2024]\r\n"
#define RPL_MYINFO(nick) ":ft_irc 004 " + nick + " :FT_IRC 1.0 contributes: oaoba yushimom stakimot \r\n"

// ニックネーム変更メッセージ
#define RPL_NICK(oldnick, newnick) ":" + oldnick + " NICK :" + newnick + "\r\n"

// Commandのメッセージ
#define RPL_NONE(message) ":ft_irc 300 * :" + message + "\r\n"

// CAP LSメッセージ (CAPコマンドのレスポンス)
#define CAP_LS ":ft_irc CAP * LS\r\n"

// エラーメッセージ
#define ERR_UNKNOWNCOMMAND(nick, command) ":ft_irc 421 " + nick + " " + command + " :Unknown command\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ":ft_irc 432 " + nick + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, nickname) ":ft_irc 433 " + nick + " " + nickname + " :Nickname is already in use\r\n"
#define ERR_NOTREGISTERED(nick) ":ft_irc 451 " + nick + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) ":ft_irc 461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick) ":ft_irc 462 " + nick + " :You may not reregister\r\n"

#define ERR_CHANOPRIVSNEEDED(nick, ch_name) "482 " + nick + " " + ch_name + " : You're not channel operator\r\n"
#define ERR_NOSUCHCHANNEL(nick) "403 " + nick + "#nonexistent :No such channel\r\n"
#define ERR_NOTONCHANNEL(nick, ch_name) "442 " + nick + " " + ch_name + " :You're not on that channel\r\n"
#define ERR_USERNOTINCHANNEL(nick, ch_name) "441 kicker " + nick + " " + ch_name + " :They aren't on that channel\r\n"
#define ERR_PASSWDMISMATCH(nick) "464 " + nick + " :Password incorrect\r\n"

// パスワードエラーメッセージ
#define PASS_ERROR(host) "ERROR :Closing Link: " + host + "(Bad Password)\r\n"

#define JOIN_SCCESS_MSG(nick, user, ch_name) nick + "! " + user + "JOIN : " + ch_name + "\r\n"
#define ERR_CHANNELISFULL(nick, ch_name) "ERR_CHANNELISFULL (471) " + nick + " "+ ch_name + " :Cannot join channel (+l)\r\n"
#define ERR_BADCHANNELKEY(nick, ch_name)  "ERR_BADCHANNELKEY (475) " + nick + " " + ch_name + " :Cannot join channel (+k)\r\n"
#define ERR_INVITEONLYCHAN(nick, ch_name) "ERR_INVITEONLYCHAN (473)  " + nick + " " + ch_name + " :Cannot join channel (+l)\r\n"

namespace Command{
    void PASS(Client &client, Server *server, const Message &message);
    void NICK(Client &client, std::map<std::string, int> &map_nick_fd, const Message &message);
    void KICK(Client &client, Server *server, const Message &message);
    void JOIN(Client &client, Server *server, const Message &message);
    void USER(Client &client, const Message &message);
	void CAP(Client &client, std::vector<struct pollfd> &pollfds,
			 std::map<int, Client> &users, std::map<std::string, int> &nick_to_fd,
			 const Message &message);
};

void SendMessage(int fd, const std::string &message, int flag);
void SendWelcomeMessage(const Client &client);
void ClearClientInfo(
		Client &client,
		std::vector<struct pollfd> &pollfds,
		std::map<int, Client> &users,
		std::map<std::string, int> &nick_to_fd
);

#endif
