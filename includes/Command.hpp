#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"

#define JOIN_SCCESS_MSG(nick, ch_name) nick + "! JOIN : " + ch_name + "\r\n"
#define ERR_CHANNELISFULL(nick, ch_name) "ERR_CHANNELISFULL (471) " + nick + " "+ ch_name + " :Cannot join channel (+l)\r\n";
#define ERR_BADCHANNELKEY(nick, ch_name)  "ERR_BADCHANNELKEY (475) " + nick + " " + ch_name + " :Cannot join channel (+k)\r\n";
#define ERR_INVITEONLYCHAN(nick, ch_name) "ERR_INVITEONLYCHAN (473)  " + nick + " " + ch_name + " :Cannot join channel (+l)\r\n";

class Server;
class Client;
class Message;

// エラーメッセージ
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters\r\n"

namespace Command{
    void PASS(Client &client, Server *server, const Message &message);
    void JOIN(Client &client, Server *server, const Message &message);

};

void SendMessage(int fd, const std::string &message, int flag);

#endif
