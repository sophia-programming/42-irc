#include "Command.hpp"

void Command::INVITE(Client &client, Server *server, const Message &message)
{
	std::vector<std::string> msg = message.GetParams();
	std::string msg_to_c;

	if(msg.size() < 2){
		msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname());
        //paramが不足していたらエラー
        return ;
    }
	std::string invited = msg[0];
	std::string ch_name = msg[1];
	std::string inviter = client.GetNickname();

	if(/*invitedがサーバーのメンバーにいるか*/){
		msg_to_c = ERR_NOSUCHNICK(inviter, invited);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}
	Channel* ch = server->GetChannel(ch_name);
	if(!ch){
		msg_to_c = ERR_NOSUCHNICK(inviter, invited);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}
	Client* cl_inviter = ch->GetUser(inviter);
	if(!cl_inviter){
		msg_to_c = ERR_NOTONCHANNEL(inviter, ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;

	}else if(ch->GetPriv(inviter) != P_Operator){
		msg_to_c = ERR_CHANOPRIVSNEEDED(inviter, ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}
	Client* cl_invited = ch->GetUser(invited);
	if(cl_invited){
		msg_to_c = ERR_USERONCHANNEL(inviter, invited, ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}

	ch->AddUserinInvite(invited);
	msg_to_c = INVITE_SUCCESS(inviter, client.GetUsername(), client.GetIpAdress(), ch_name);
	SendMessage(client.GetFd(), msg_to_c, 0);
}
