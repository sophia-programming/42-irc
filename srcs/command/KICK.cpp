#include "Command.hpp"
#include "Channel.hpp"

void Command::KICK(Client &client, Server *server, const Message &message)
{
    if(message.GetParams().size() < 2){
        //paramが不足していたらエラー
        return ;
    }

    std::string ch_name = message.GetParams()[0];
    std::string user_name = message.GetParams()[1];
    std::string msg_to_c;
    std::string msg_to_all;
    // std::cout << "ch " + ch_name << " user " + user_name << std::endl;

    Channel* ch = server->GetChannel(ch_name);
    Client* user = ch->GetUser(user_name);
    if(!ch){
        msg_to_c = ERR_NOSUCHCHANNEL(ch_name);
        SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
        // doesn't exist
    }
    else if(ch->GetPriv(client.GetNickname())  != P_Operator){
        msg_to_c = ERR_CHANOPRIVSNEEDED(client.GetNickname(), ch_name);
        SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
        //you need oprator priv
    }
    else if(!user){
        msg_to_c = ERR_USERNOTINCHANNEL(client.GetNickname(), ch_name);
        SendMessage(client.GetFd(), msg_to_c, 0);
        return ;
        // user donsent exist 
    }
    msg_to_all = client.GetNickname() + "! KICK " + ch_name + " "+ user_name;
    
    if(message.GetParams().size() == 3){
        msg_to_all = client.GetNickname() + "! KICK " + ch_name + " "+ user_name + message.GetParams()[2]; 
    }
    ch->SendMsgToAll(msg_to_all);
    ch->RmUser(user);
    ch->RmUserFromInvite(user_name);
}
