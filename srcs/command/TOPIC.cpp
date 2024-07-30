#include "Command.hpp"
#include "Channel.hpp"

/* TOPICコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::TOPIC(Client &client, Server *server, const Message &message)
{
	if(message.GetParams().size() < 1){
		//paramが不足していたらエラー
		return ;
	}
	std::string ch_name = message.GetParams()[0];
	std::string topic;
	std::string msg_to_c;
	std::string msg_to_all;

	Channel* ch = server->GetChannel(ch_name);

	// チャンネルが存在しない場合
	if(!ch){
		std::cout << "!ch" << std::endl;
		msg_to_c = ERR_NOSUCHCHANNEL(client.GetNickname());
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
		// doesn't exist
	}
	// クライアントがチャンネルに参加していない場合
	else if(ch->GetUser(client.GetNickname()) == nullptr){
		std::cout << "!user" << std::endl;
		msg_to_c = ERR_NOTONCHANNEL(client.GetNickname(), ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	//チャンネルのトピックを設定
	if(message.GetParams().size() == 2 && message.GetParams()[1] != ""){
		topic = message.GetParams()[1];
		ch->SetToic(topic);
		msg_to_c = ":ft_irc 332 " + client.GetNickname() + " " + ch_name + " :"+ topic + "\n";
		msg_to_all = ":" + client.GetNickname() + "!" + client.GetHostname() + " TOPIC "+ ch_name + ":" + topic + "\n";
		SendMessage(client.GetFd(), msg_to_c, 0);
		ch->SendMsgToAll(msg_to_all);
	}
	//チャンネルのトピックを削除
	else if (message.GetParams().size() == 2 && message.GetParams()[1] == "") {
		ch->SetToic("non");
		std::cout << "remove" << std::endl;
		msg_to_all = ":ft_irc 332 " + client.GetNickname() + " " + ch_name + " :No topic is set\n";
		ch->SendMsgToAll(msg_to_all);
	}
	// チャンネルのトピックを取得
	else{
		if (ch->GetTopic() == "non")
			msg_to_c = "ft_irc 331 " + client.GetNickname() + " " + ch_name + " :No topic is set\n";
		else
			msg_to_c = "ft_irc 332 " + client.GetNickname() + " " + ch_name + " :"+ ch->GetTopic() + "\n";
		SendMessage(client.GetFd(), msg_to_c, 0);
	}
}