#include "Command.hpp"
#include "Message.hpp"

void show_topic(Channel *ch, const std::string &nick, std::string &msg_to_c){
    if (ch->GetTopic().empty()){
        msg_to_c += ":ft_irc 331 " + nick + " " + ch->GetName() + " : No topic is set" + "\n";
    }
    else{
        msg_to_c += ":ft_irc 332 " + nick + " " + ch->GetName() + " :" + ch->GetTopic() + "\n";
    }
}

void show_channel_member(Channel *ch, const std::string &nick, std::string &msg_to_c){
    std::map<Client*, User_Priv>::iterator iter = ch->users_.begin();

    msg_to_c += ":server 353 " + nick + " = " + ch->GetName() + " :" ;
    while(iter != ch->users_.end()){
        msg_to_c += iter->first->GetNickname() + " ";
        iter++;
    }
    msg_to_c += "\n:server 366 " + nick + " " + ch->GetName() + " :End of list\n";
}

void join_without_key(Channel *ch, Client &client, std::string& msg_to_c){

    if(ch->GetLimit() <= ch->users_.size()){ // エラー１すでに満員
        msg_to_c = ERR_CHANNELISFULL(client.GetNickname(), ch->GetName());
    }
    else if(ch->GetModeKey() == true){ //エラー２keyが必要
        msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
    }
    else if(ch->GetModeInvite() == true){
        if(ch->IsInvited(client.GetNickname()) == false){ //エラー３招待が必要
            msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch->GetName());
        }
        else{
            msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
            ch->AddUserAsN(client);
        }
    }
    else{
        msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
        ch->AddUserAsN(client);
    }
}


void join_with_key(Channel *ch, Client &client, std::string& msg_to_c, const std::string& key){
    if(ch->GetLimit() <= ch->users_.size()){ // エラー１すでに満員
        msg_to_c = ERR_CHANNELISFULL(client.GetNickname(), ch->GetName());
    }
    else if(ch->GetModeKey() == true){
        if(ch->GetKey() != key){ //エラー２keyが一致しない
            msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
        }else{
            ch->AddUserAsN(client);
        }
    }
    else if(ch->GetModeInvite() == true){
        if(ch->IsInvited(client.GetNickname()) == false){ //エラー３招待が必要
            msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch->GetName());
        }
        else{
            msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
            ch->AddUserAsN(client);
        }
    }
    else{
        msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
            ch->AddUserAsN(client);
    }
}


// JOIN コマンドの処理をする関数
// 1: クライアント情報
// 2: Server情報
// 3: 受け取ったメッセージ
void Command::JOIN(Client &client, Server *server, const Message &message)
{
    Channel* ch;
    const std::vector<std::string> msg = message.GetParams();
    if(msg.size() < 1){
        return ;
    }
    // クライアントに送信するメッセージ
    std::string msg_to_c;

    // チャンネル名
    const std::string ch_name = msg[0];

    if(msg.size() == 1){
        try{
            if(server->IsChannel(ch_name)){ //チャンネルが存在するとき
                // 指定されているチャンネルの取得
                Channel* ch = server->FindChannelByName(ch_name);
                if(ch->GetLimit() <= ch->users_.size()){ // エラー1 すでに満員
                    msg_to_c = ERR_CHANNELISFULL(client.GetNickname(), ch->GetName());
                    SendMessage(client.GetFd(), msg_to_c, 0);
                    return ;
                }
                if(ch->GetModeKey() == true){ //エラー２keyが必要
                    msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
                    SendMessage(client.GetFd(), msg_to_c, 0);
                    return ;
                }
                if(ch->GetModeInvite() == true){ //エラー３招待が必要
                    if(ch->IsInvited(client.GetNickname()) == false){ //招待されてるかの確認
                        msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch->GetName());
                        SendMessage(client.GetFd(), msg_to_c, 0);
                        return ;
                    }

                    msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
                    ch->AddUserAsN(client);
                }

                msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
                ch->AddUserAsN(client);
            }
            else{ // チャンネルが存在しないとき
                ch = server->CreateChannel(ch_name);
                if(!ch){
                    return;
                }
                ch->AddUserAsO(client);
                msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
            }
        }catch(const std::exception& e){
            std::cerr << "Exception caught: " << e.what() << std::endl;
            return;
        }

    }
    else if (msg.size() == 2){ //チャンネルkeyが指定されているとき
        try{
            const std::string key = msg[1];
            if(server->IsChannel(ch_name)){ //チャンネルが存在するとき
                // 指定されているチャンネルの取得
                Channel* ch = server->FindChannelByName(ch_name);
                if(ch->GetLimit() <= ch->users_.size()){ // エラー１すでに満員
                    msg_to_c = ERR_CHANNELISFULL(client.GetNickname(), ch->GetName());
                    SendMessage(client.GetFd(), msg_to_c, 0);
                    return ;
                }
                if(ch->GetModeKey() == true){ //エラー２keyが必要
                    if(ch->GetKey() != key){ //エラー２keyが一致しない
                        msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
                        SendMessage(client.GetFd(), msg_to_c, 0);
                        return ;
                    }
                    ch->AddUserAsN(client);
                }
                if(ch->GetModeInvite() == true){ //エラー３招待が必要
                    if(ch->IsInvited(client.GetNickname()) == false){ //招待されてるかの確認
                        msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch->GetName());
                        SendMessage(client.GetFd(), msg_to_c, 0);
                        return ;
                    }
                    msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
                    ch->AddUserAsN(client);
                }

                msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
                ch->AddUserAsN(client);
            }
            else{
                ch = server->CreateChannel(ch_name);
                msg_to_c = client.GetNickname() +"! JOIN :" + ch->GetName();
                if(!ch){
                    return ;
                }
                ch->AddUserAsO(client);
                msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(),client.GetUsername(), ch->GetName());
                ch->SetKey(key);
                    // ch->SetMode(CM_Key);
            }
        }catch(const std::exception& e){
            std::cerr << "Exception caught: " << e.what() << std::endl;
            SendMessage(client.GetFd(), "Error\n", 0);
            return;
        }
    }
    show_topic(ch,client.GetNickname(), msg_to_c);
    show_channel_member(ch, client.GetNickname(), msg_to_c);
    SendMessage(client.GetFd(), msg_to_c, 0);
}