#include "Command.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

/* TOPICコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::TOPIC(Client &client, Server *server, const Message &message)
{
	if(message.GetParams().size() < 1){
		//paramが不足していたらエラー
		std::string msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "TOPIC");
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	std::string ch_name = message.GetParams()[0];
	std::string topic;
	std::string msg_to_c;
	std::string msg_to_all;

	Channel* ch = server->FindChannelByName(ch_name);

	// チャンネルが存在しない場合
	if(!ch){
		msg_to_c = ERR_NOSUCHCHANNEL(client.GetNickname());
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
		// doesn't exist
	}
	// クライアントがチャンネルに参加していない場合
	else if(ch->GetUser(client.GetNickname()) == NULL){
		msg_to_c = ERR_NOTONCHANNEL(client.GetNickname(), ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	//チャンネルのトピックを設定
	if(message.GetParams().size() == 2 && message.GetParams()[1] != ""){
		//channelがtモードかつ、クライアントがオペレーター権限を持っていない場合
		if(ch->GetModeTopic() == true && ch->GetPriv(client.GetNickname()) != P_Operator){
			msg_to_c = ERR_CHANOPRIVSNEEDED(client.GetNickname(), ch_name);
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		topic = message.GetParams()[1];
		ch->SetToic(topic);
		ch->SetTopicSetter(&client);
		ch->SetTopicTime(std::time(NULL));
		msg_to_all = ":ft_irc 332 " + client.GetNickname() + " " + ch_name + " :"+ topic + "\n" +
		":ft_irc 333 " + client.GetNickname() + " " + ch_name + " " + client.GetNickname() + " " + change_string(ch->GetTopicTime()) + "\n";
		ch->SendMsgToAll(msg_to_all, &client);
		SendMessage(client.GetFd(), msg_to_all, 0);
	}
	//チャンネルのトピックを削除
	else if (message.GetParams().size() == 2 && message.GetParams()[1] == "") {
		//channelがtモードかつ、クライアントがオペレーター権限を持っていない場合
		if(ch->GetModeTopic() == true && ch->GetPriv(client.GetNickname()) != P_Operator){
			msg_to_c = ERR_CHANOPRIVSNEEDED(client.GetNickname(), ch_name);
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		ch->SetToic("non");
		ch->SetTopicSetter(&client);
		// std::cout << "remove" << std::endl;
		msg_to_all = ":ft_irc 332 " + client.GetNickname() + " " + ch_name + " :No topic is set\n" +
			":ft_irc 333 " + client.GetNickname() + " " + ch_name + " " + ch->GetTopicSetter()->GetNickname() + " " + change_string(ch->GetTopicTime()) + "\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	// チャンネルのトピックを取得
	else{
		if (ch->GetTopic() == "non")
		{
			msg_to_c = "ft_irc 331 " + client.GetNickname() + " " + ch_name + " :No topic is set\n";
			SendMessage(client.GetFd(), msg_to_c, 0);
		}
		else
		{
			msg_to_all = ":ft_irc 332 " + client.GetNickname() + " " + ch_name + " :"+ ch->GetTopic() + "\n" +
				":ft_irc 333 " + client.GetNickname() + " " + ch_name + " " + ch->GetTopicSetter()->GetNickname() + " " + change_string(ch->GetTopicTime()) + "\n";
			SendMessage(client.GetFd(), msg_to_all, 0);
			ch->SendMsgToAll(msg_to_all, &client);
		}
	}
}
