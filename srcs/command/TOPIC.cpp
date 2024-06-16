// #include "Command.hpp"

// void Command::TOPIC(Client &client, Server *server, const Message &message)
// {
// 	if(message.GetParams().size() < 1){
// 		//paramが不足していたらエラー
// 		return ;
// 	}

// 	std::string ch_name = message.GetParams()[0];
// 	std::string topic;
// 	std::string msg_to_c;
// 	std::string msg_to_all;

// 	Channel* ch = server->GetChannel(ch_name);
// 	if(!ch){
// 		msg_to_c = ERR_NOSUCHCHANNEL(client.GetNickname());
// 		SendMessage(client.GetFd(), msg_to_c, 0);
// 		return ;
// 		// doesn't exist
// 	}
// 	else if(ch->GetPriv(client.GetUsername())  == P_None){
// 		msg_to_c = ERR_NOTONCHANNEL(client.GetNickname(), ch_name);
// 		SendMessage(client.GetFd(), msg_to_c, 0);
// 		return ;
// 		//you need oprator priv
// 	}
// 	if(message.GetParams().size() == 2){
// 		topic = message.GetParams()[1];
// 		ch->SetTopic(topic);
// 		msg_to_all = client.GetNickname() + "! TOPIC " + ch_name + " :"+ topic;
// 		ch->SendMsgToAll(msg_to_all);
// 	}
// 	else{
// 		msg_to_all = client.GetNickname() + "! TOPIC " + ch_name + " :"+ ch->GetTopic();
// 		ch->SendMsgToAll(msg_to_all);
// 	}
// }