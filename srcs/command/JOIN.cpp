#include "Command.hpp"
#include "Message.hpp"

//JOINに成功した後にチャンネルのトピックとメンバーリストを表示する関数
// 1:Channel *ch -> チャンネルオブジェクト
// 2:const std::string &nick -> コマンド実行したクライアントのニックネーム
// 3:std::string &msg_to_c -> クライアントに送信するメッセージ
void show_topic_and_member(Channel *ch, const std::string &nick, std::string &msg_to_c){
    if (ch->GetTopic().empty()){
        msg_to_c += ":ft_irc 331 " + nick + " " + ch->GetName() + " : No topic is set" + "\r\n";
    }
    else{
        msg_to_c += ":ft_irc 332 " + nick + " " + ch->GetName() + " :" + ch->GetTopic() + "\r\n";
    }
    std::map<Client*, User_Priv>::iterator iter = ch->users_.begin();
    msg_to_c += ":server 353 " + nick + " = " + ch->GetName() + " :" ;
    while(iter != ch->users_.end()){
        msg_to_c += iter->first->GetNickname() + " ";
        iter++;
    }
    msg_to_c += "\n:server 366 " + nick + " " + ch->GetName() + " :End of list\r\n";
}
/*
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
*/

// JOIN コマンドの処理をする関数
// 1: クライアント情報
// 2: Server情報
// 3: 受け取ったメッセージ
void Command::JOIN(Client &client, Server *server, const Message &message)
{
    Channel* ch;
    std::vector<std::string> msg = message.GetParams();
    if(msg.size() < 1){
        return ;
    }
    int client_fd = client.GetFd();
    // クライアントに送信するメッセージ
    std::string msg_to_c;

    // チャンネル名
    const std::string ch_name = RmRFromString(msg[0]);
    if(msg.size() == 1){
        try{
            if(server->IsChannel(ch_name)){ //チャンネルが存在するとき
                // 指定されているチャンネルの取得
                Channel* ch = server->FindChannelByName(ch_name);
                if(ch->GetLimit() <= ch->users_.size()){ // エラー1 すでに満員
                    msg_to_c = ERR_CHANNELISFULL(client.GetNickname(), ch->GetName());
                    SendMessage(client_fd, msg_to_c, 0);
                    return ;
                }
                if(ch->GetModeKey() == true){ //エラー２keyが必要
                    msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
                    SendMessage(client_fd, msg_to_c, 0);
                    return ;
                }
                if(ch->GetModeInvite() == true){ //エラー３招待が必要
                    if(ch->IsInvited(client.GetNickname()) == false){ //招待されてるかの確認
                        msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch->GetName());
                        SendMessage(client_fd, msg_to_c, 0);
                        return ;
                    }
                    msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                    ch->AddUserAsN(client);
                }
                msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                ch->AddUserAsN(client);
                show_topic_and_member(ch,client.GetNickname(), msg_to_c);
            }
            else{ // チャンネルが存在しないとき
                ch = server->CreateChannel(ch_name);
                if(!ch){
                    return;
                }
                msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                msg_to_c += GIVE_OP_PRIV(client.GetNickname(), client.GetUsername(), client.GetHostname(), ch_name, client.GetNickname());
                ch->AddUserAsO(client);
                std::cout<< "print ch name " << ch->GetName() << std::endl;
                show_topic_and_member(ch,client.GetNickname(), msg_to_c);
            }
        }catch(const std::exception& e){
            msg_to_c = ERR_BADCHANMASK(client.GetNickname(), ch_name);
            SendMessage(client_fd, msg_to_c, 0);
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
                    SendMessage(client_fd, msg_to_c, 0);
                    return ;
                }
                if(ch->GetModeKey() == true){
                    if(ch->GetKey() != key){ //エラー２keyが一致しない
                        msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
                        SendMessage(client_fd, msg_to_c, 0);
                        return ;
                    }
                    msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                    ch->AddUserAsN(client);
                    show_topic_and_member(ch,client.GetNickname(), msg_to_c);
                }
                if(ch->GetModeInvite() == true){ //エラー３招待が必要
                    if(ch->IsInvited(client.GetNickname()) == false){ //招待されてるかの確認
                        msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch->GetName());
                        SendMessage(client_fd, msg_to_c, 0);
                        return ;
                    }
                    msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                    ch->AddUserAsN(client);
                    show_topic_and_member(ch,client.GetNickname(), msg_to_c);
                }
                msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                ch->AddUserAsN(client);
                show_topic_and_member(ch,client.GetNickname(), msg_to_c);
            }
            else{
                ch = server->CreateChannel(ch_name);
                if(!ch){
                    return ;
                }
                msg_to_c = JOIN_SUCCESS_MSG(client.GetNickname(),client.GetUsername(), client.GetHostname() ,ch->GetName());
                msg_to_c += GIVE_OP_PRIV(client.GetNickname(), client.GetUsername(), client.GetHostname(), ch_name, client.GetNickname());
                ch->AddUserAsO(client);
                show_topic_and_member(ch,client.GetNickname(), msg_to_c);
                ch->SetKey(key);
                ch->SetModeKey(true);
            }
        }catch(const std::exception& e){
            msg_to_c = ERR_BADCHANMASK(client.GetNickname(), ch_name);
            SendMessage(client_fd, msg_to_c, 0);
            return;
        }
    }
    SendMessage(client_fd, msg_to_c, 0);
}
