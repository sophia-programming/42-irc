#include "Command.hpp"
#include "Message.hpp"

// JOIN コマンドの処理をする関数
void Command::JOIN(Client &client, Server *server, Message &message)
{
    int fd = client.GetFd();
    const std::vector<std::string> msg = message.GetParams();
    std::string msg_to_c;

    if(msg.size() == 1){
        if(server->ChannelExist(msg[0])){
            //チャンネルが存在するとき
            Channel tmp = server->GetChannel(msg[0]);
            if(tmp.CheckMode(CM_Key)){
                msg_to_c = "ERR_BADCHANNELKEY (475) - " + tmp.GetName() + " :Cannot join channel (+k)\n";
            }
            if(tmp.GetLimit() <= tmp.users_.size()){
            }

        }
        else{
            // チャンネルが存在しないとき
            server->CreateChannel(msg[0]);
            server->GetChannel(msg[0]).AddUserAsO(client);
        }
    }
    else{ //チャンネルkeyが指定されているとき
        if(server->ChannelExist(msg[0])){
            server->GetChannel(msg[0]).AddUserAsN(client);
        }
        else{
            server->CreateChannel(msg[0]);
            Channel tmp = server->GetChannel(msg[0]);
            tmp.AddUserAsO(client);
            tmp.SetKey(msg[1]);
            tmp.SetMode(CM_Key);
        }

    }
}