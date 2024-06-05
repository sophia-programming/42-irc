#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Message.hpp"
#include "Client.hpp"

class Server;
class Client;
class Message;
// エラーメッセージ
#define ERR_UNKNOWNCOMMAND(nick, command) "421 " + nick + " " + command + " :Unknown command\r\n"
#define ERR_ERRONEUSNICKNAME(nick, nickname) "432 " + nick + " " + nickname + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, nickname) "433 " + nick + " " + nickname + " :Nickname is already in use\r\n"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters\r\n"

#define ERR_CHANOPRIVSNEEDED(nick, ch_name) "482 " + nick + " " + ch_name + " : You're not channel operator\r\n"
#define ERR_NOSUCHCHANNEL(nick) "403 " + nick + "#nonexistent :No such channel\r\n"
#define ERR_NOTONCHANNEL(nick, ch_name) "442 " + nick + " " + ch_name + " :You're not on that channel\r\n"
#define ERR_USERNOTINCHANNEL(nick, ch_name) "441 kicker " + nick + " " + ch_name + " :They aren't on that channel\r\n"

namespace Command{
    void PASS(Client &client, Server *server, const Message &message);
    void NICK(Client &client, std::map<std::string, int> &map_nick_fd, const Message &message);
    
    void KICK(Client &client, Server *server, const Message &message);
};

void SendMessage(int fd, const std::string &message, int flag);

#endif
