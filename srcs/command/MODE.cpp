#include "Command.hpp"
#include "Channel.hpp"

void Command::MODE(Client &client, Server *server, const Message &message)
{
	std::string msg_to_c;

	//sevser側 テスト出力
	std::cout << "MODE" << " " <<  client.GetNickname() << std::endl;

	//parmsがない場合(MODEの引数がない)場合->エラー
	if(message.GetParams().size() < 2){
		msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "mode");
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}

	std::string ch_name = message.GetParams()[0];
	std::string mode_option = message.GetParams()[1];
	std::string msg_to_all;

	std::cout << mode_option << std::endl << "mode_option "<< mode_option <<  std::endl;

	Channel* ch = server->FindChannelByName(ch_name);

	if(!ch){  //エラー１　指定されたチャンネルが存在しない
		msg_to_c = ERR_NOSUCHCHANNEL(ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	else if(ch->GetUser(client.GetNickname()) == NULL){ //エラー２　チャンネルに参加していない
		msg_to_c = ERR_NOTONCHANNEL(client.GetNickname(), ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	else if(message.GetParams().size() == 1){ //オプションなし channelのmodeを確認
		msg_to_c = ":ft_irc 324 " + client.GetNickname() + " " + ch_name;
		if (ch->GetModeInvite() || ch->GetModeTopic() || ch->GetModeIsLimit() || ch->GetModeKey()){
			msg_to_c += " +";
		}
		if(ch->GetModeInvite()){
			msg_to_c += "i";
		}
		if(ch->GetModeTopic()){
			msg_to_c += "t";
		}
		if(ch->GetModeIsLimit()){
			msg_to_c += "l";
		}
		if(ch->GetModeKey()){
			msg_to_c += "k";
		}
		if(ch->GetModeIsLimit())
		msg_to_c += " " + change_string(ch->GetModeLimit());
		msg_to_c += "\r\n";
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	else if(ch->GetPriv(client.GetNickname())  != P_Operator){ //エラー２　コマンド実行権限なし
		msg_to_c = ERR_CHANOPRIVSNEEDED(client.GetNickname(), ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
	}
	//オペレーター権限付与
	if(mode_option == "+o"){
		if(message.GetParams().size() < 3){
			msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "mode");
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		std::string user_name = message.GetParams()[2];
		Client* user = ch->GetUser(user_name);
		if(!user){ //エラー３　指定したユーザーがチャンネルにいない
			// msg_to_c = ERR_USERNOTINCHANNEL(client.GetNickname(), ch_name);
			msg_to_c = "403 " + client.GetNickname() + " " + ch_name + " No such nick/channel\r\n";
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		ch->SetPrivAsOperator(user->GetNickname());
		msg_to_all = ":" + client.GetNickname() + "!" + client.GetHostname() + " MODE " + ch_name + " +o "+ user_name + "\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
		// SendMessage(client.GetFd(), msg_to_all, 0);
		// ↑mergeしたら復活
	}
	//オペレーター権限剥奪
	else if(mode_option == "-o"){
		if(message.GetParams().size() < 3){
			msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "mode");
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		std::string user_name = message.GetParams()[2];
		Client* user = ch->GetUser(user_name);
		if(!user){ //エラー３　指定したユーザーがチャンネルにいない
			msg_to_c = "403 " + client.GetNickname() + " " + ch_name + " No such nick/channel\r\n";
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		ch->SetPrivAsNomal(user->GetNickname());
		msg_to_all = ":" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " -o "+ user_name + "\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
		// SendMessage(client.GetFd(), msg_to_all, 0);
		// ↑mergeしたら復活
	}
	//招待制モードに変更
	else if(mode_option == "+i"){
		ch->SetModeInvite(true);
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " +i\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	//招待制モード解除
	else if(mode_option == "-i"){
		ch->SetModeInvite(false);
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " -i\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	//チャンネルをトピック保護モードに変更（オペレーターのみがトピック編集可能）
	else if(mode_option == "+t"){
		ch->SetModeTopic(true);
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " +t\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	//チャンネルのトピック保護モードを解除
	else if(mode_option == "-t"){
		ch->SetModeTopic(false);
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " -t\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	//チャンネルにキーを設定
	else if(mode_option == "+k"){
		if(message.GetParams().size() < 3){
			msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "mode");
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		std::string key = message.GetParams()[2];
		ch->SetKey(key);
		ch->SetModeKey(true);
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " +k " + key + "\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	//チャンネルのキーを解除
else if(mode_option == "-k"){
		ch->SetKey("");
		ch->SetModeKey(false);
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " -k\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
	else if(mode_option == "+l"){ //人数制限設定
		if(message.GetParams().size() < 3){
			msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "mode");
			SendMessage(client.GetFd(), msg_to_c, 0);
			return ;
		}
		try{
			std::string limit = message.GetParams()[2];
			//リミットが1以下の場合
			if (string_to_lint(limit) < 1){
				msg_to_c = ERR_LIMITVALUEMINUS(client.GetNickname(), ch_name);
				SendMessage(client.GetFd(), msg_to_c, 0);
				return ;
			}
			//リミットがLONG_MAXを超える場合
			if (string_to_lint(limit) > LONG_MAX){
				msg_to_c = ERR_LIMITVALUEOVER(client.GetNickname(), ch_name);
				SendMessage(client.GetFd(), msg_to_c, 0);
				return ;
			}
			ch->SetModeIsLimit(true);
			ch->SetModeLimit(string_to_lint(limit));
			std::cout << "ch->limit:" << ch->GetModeLimit() << std::endl;
			msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " +l " + limit + "\r\n";
			ch->SendMsgToAll(msg_to_all, &client);
		}catch(const std::invalid_argument& e){
			msg_to_c = ERR_NEEDMOREPARAMS(client.GetNickname(), "mode");
			SendMessage(client.GetFd(), msg_to_c, 0);
		}
	}
	else if(mode_option == "-l"){ //人数制限解除
		ch->SetModeIsLimit(false);
		ch->SetModeLimit(0);
		msg_to_all = client.GetNickname() + "! MODE " + ch_name + "-l";
		msg_to_all = ":nick!" + client.GetNickname() + client.GetHostname() + " MODE " + ch_name + " -l\r\n";
		ch->SendMsgToAll(msg_to_all, &client);
	}
}
