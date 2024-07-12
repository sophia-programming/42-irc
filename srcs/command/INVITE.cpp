#include "Command.hpp"
// INVITE コマンドの処理をする関数
// 1: Client &client -> クライアント情報
// 2: Server *server -> Server情報
// 3: const Message &message -> 受け取ったメッセージ
void Command::INVITE(Client &client, Server *server, const Message &message)
{
	std::vector<std::string> msg = message.GetParams();
	std::string msg_to_c;

	std::cout << "msg size "  << msg.size() << std::endl;
	if(msg.size() < 2){
        //paramが不足していたらエラー
		std::cout << "aaaa" << std::endl;
		msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "INVITE");
		SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
    }
	std::string invited = msg[0];
	std::string ch_name = msg[1];
	std::string inviter = client.GetNickname();
	Client* invited_c = server->FindClientByNickname(invited);

	if(!invited_c){ // エラー１　招待された人がサーバーにいない
		msg_to_c = ERR_NOSUCHNICK(inviter, invited);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}
	Channel* ch = server->FindChannelByName(ch_name);
	if(!ch){ // エラー２　指定されたチャンネルが存在しない
		msg_to_c = ERR_NOSUCHCHANNEL(inviter);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}
	Client* cl_inviter = ch->GetUser(inviter);
	if(!cl_inviter){ //エラー３　招待者が指定のチャンネルメンバーでない
		msg_to_c = ERR_NOTONCHANNEL(inviter, ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;

	}else if(ch->GetPriv(inviter) != P_Operator){ //エラー４　招待者の権限不足
		msg_to_c = ERR_CHANOPRIVSNEEDED(inviter, ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}
	Client* cl_invited = ch->GetUser(invited);
	if(cl_invited){ // エラー５　招待者がチャンネルにすでに存在する
		msg_to_c = ERR_USERONCHANNEL(inviter, invited, ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return;
	}

	ch->AddUserinInvite(invited);
	msg_to_c = INVITE_SUCCESS(inviter, client.GetUsername(), client.GetIpAdress(), invited, ch_name);
	SendMessage(client.GetFd(), msg_to_c, 0);
	SendMessage(invited_c->GetFd(), msg_to_c, 0);
}
