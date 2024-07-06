#include "Command.hpp"
#include "Message.hpp"

// JOIN コマンドの処理をする関数
// 1: クライアント情報
// 2: Server情報
// 3: 受け取ったメッセージ
void Command::JOIN(Client &client, Server *server, const Message &message)
{
    const std::vector<std::string> msg = message.GetParams();
    if(msg.size() < 1){
        return ;
    }
    // クライアントに送信するメッセージ
    std::string msg_to_c;
    std::cout <<"msg size " << msg.size() << " [1] " << msg[0] << "\n" << "2 " << msg[1] << std::endl;

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
                if(ch->CheckMode(CM_Key)){ //エラー２keyが必要
                    msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
                    SendMessage(client.GetFd(), msg_to_c, 0);
                    return ;
                }
                if(ch->CheckMode(CM_Invite)){
                    if(ch->IsInvited(client.GetNickname()) == false){ //エラー３招待が必要
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
                Channel* ch = server->CreateChannel(ch_name);
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
                if(ch->CheckMode(CM_Key)){
                    if(ch->GetKey() != key){ //エラー２keyが一致しない
                        msg_to_c = ERR_BADCHANNELKEY(client.GetNickname(), ch->GetName());
                        SendMessage(client.GetFd(), msg_to_c, 0);
                        return ;
                    }
                    ch->AddUserAsN(client);
                }
                if(ch->CheckMode(CM_Invite)){
                    if(ch->IsInvited(client.GetNickname()) == false){ //エラー３招待が必要
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
                Channel* ch = server->CreateChannel(ch_name);
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
            return;
        }
    }
    SendMessage(client.GetFd(), msg_to_c, 0);
}