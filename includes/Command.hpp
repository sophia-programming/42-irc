#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"

// エラーメッセージ
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters\r\n"

namespace Command{
    void PASS(Client &client, Server *server, const Message &message);
    void JOIN(Client &client, Server *server, const Message &message);

};

void SendMessage(int fd, const std::string &message, int flag);

#endif
