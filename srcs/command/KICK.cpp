#include "Command.hpp"
#include "Channel.hpp"

void Command::KICK(Client &client, Server *server, const Message &message)
{
    std::string msg_to_c;
     std::vector<std::string> msg = message.GetParams();
    if(msg.size() < 2){
        //paramが不足していたらエラー
        msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "kick");
		SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
    }

    std::string ch_name = RmRFromString(msg[0]);
    std::string user_name = RmRFromString(msg[1]);
    std::string msg_to_all;

    Channel* ch = server->FindChannelByName(ch_name);
    if(!ch){  //エラー１　指定されたチャンネルが存在しない
        msg_to_c = ERR_NOSUCHCHANNEL(ch_name);
        SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
    }
    else if(ch->GetPriv(client.GetNickname())  != P_Operator){ //エラー２　コマンド実行権限なし
        msg_to_c = ERR_CHANOPRIVSNEEDED(client.GetNickname(), ch_name);
        SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
    }
    Client* user = ch->GetUser(user_name);
    if(!user){ //エラー３　指定したユーザーがチャンネルにいない
        msg_to_c = ERR_USERNOTINCHANNEL(client.GetNickname(), ch_name);
        SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
    }
    if(msg.size() == 3){
        msg_to_all = KICK_MSG(client.GetNickname(), client.GetUsername(), client.GetHostname(),ch_name, user_name,msg[2]);
    }else{
        std::string emptystring;
        msg_to_all = KICK_MSG(client.GetNickname(), client.GetUsername(), client.GetHostname(),ch_name, user_name, emptystring);
    }
    ch->SendMsgToAll(msg_to_all, &client);
    ch->RmUser(user);
    ch->RmUserFromInvite(user_name);
}
