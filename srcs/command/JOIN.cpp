#include "Command.hpp"
#include "Message.hpp"

// JOIN コマンドの処理をする関数
// 1: クライアント情報
// 2: Server情報
// 3: 受け取ったメッセージ
void Command::JOIN(Client &client, Server *server, const Message &message)
{
    const std::vector<std::string> msg = message.GetParams();
    // クライアントに送信するメッセージ
    std::string msg_to_c;

    if(msg.size() < 1){
        return ;
    }
    // チャンネル名
    const std::string ch_name = msg[0];

    if(msg.size() == 1){

        if(server->ChannelExist(ch_name)){ //チャンネルが存在するとき
            // 指定されているチャンネルの取得
            Channel ch = server->GetChannel(ch_name);
            if(ch.GetLimit() <= ch.users_.size()){ // エラー１すでに満員
                msg_to_c = ERR_CHANNELISFULL(client.GetNickname(), ch.GetName());
            }
            else if(ch.CheckMode(CM_Key)){ //エラー２keyが必要
                msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch.GetName());
            }
            else if(ch.CheckMode(CM_Invite)){
                if(ch.IsInvited(client.GetNickname()) == false){ //エラー３招待が必要
                    msg_to_c = ERR_INVITEONLYCHAN(client.GetNickname(), ch.GetName());
                }
                else{
                    msg_to_c = JOIN_SCCESS_MSG(client.GetNickname(), ch.GetName());
                    ch.AddUserAsN(client);
                }
            }
            else{
                msg_to_c = client.GetNickname() +"! JOIN :" + ch.GetName();
                ch.AddUserAsN(client);
            }
        }
        else{
            // チャンネルが存在しないとき
            server->CreateChannel(ch_name);
            server->GetChannel(ch_name).AddUserAsO(client);
        }
    }
    else if (msg.size() == 2){ //チャンネルkeyが指定されているとき
        const std::string key = msg[1];
        if(server->ChannelExist(ch_name)){ //チャンネルが存在するとき
            // 指定されているチャンネルの取得
            Channel ch = server->GetChannel(ch_name);
            if(ch.GetLimit() <= ch.users_.size()){ // エラー１すでに満員
                msg_to_c = "ERR_CHANNELISFULL (471) - " + ch.GetName() + " :Cannot join channel (+l)\n";
            }
            else if(ch.CheckMode(CM_Key)){
                if(ch.GetKey() != key){ //エラー２keyが一致しない
                    msg_to_c = "ERR_BADCHANNELKEY (475) - " + ch.GetName() + " :Cannot join channel (+k)\n";
                }else{
                    ch.AddUserAsN(client);
                }
            }
            else if(ch.CheckMode(CM_Invite)){
                if(ch.IsInvited(client.GetNickname()) == false){ //エラー３招待が必要
                    msg_to_c = "ERR_INVITEONLYCHAN (473) - " + ch.GetName() + " :Cannot join channel (+l)\n";
                }
                else{
                    msg_to_c = client.GetNickname() +"! JOIN :" + ch.GetName();
                    ch.AddUserAsN(client);
                }
            }
            else{
                msg_to_c = client.GetNickname() +"! JOIN :" + ch.GetName();
                ch.AddUserAsN(client);
            }
        }
        else{
            server->CreateChannel(ch_name);
            Channel ch = server->GetChannel(ch_name);
            msg_to_c = client.GetNickname() +"! JOIN :" + ch.GetName();
            ch.AddUserAsO(client);
            ch.SetKey(key);
            ch.SetMode(CM_Key);
        }
    }
    SendMessage(client.GetFd(), msg_to_c, 0);
}