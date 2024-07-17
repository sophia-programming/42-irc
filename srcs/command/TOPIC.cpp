#include "Command.hpp"
#include "Channel.hpp"

/* TOPICコマンドの処理を行う関数
 * 引数1 -> クライアント
 * 引数2 -> サーバーの情報
 * 引数3 -> メッセージ */
void Command::TOPIC(Client &client, Server *server, const Message &message)
{
	// std::cout << "[[" << message.GetParams()[0] << "\n" << message.GetParams()[1] << "\n" << message.GetParams()[2] << "]]" << "\n";
	if(message.GetParams().size() < 1){
		//paramが不足していたらエラー
		return ;
	}

	std::string ch_name = message.GetParams()[0];
	std::string topic;
	std::string msg_to_c;
	std::string msg_to_all;

	Channel* ch = server->GetChannel("#ch\n");
	// Channel* ch = server->GetChannel(ch_name);
	// チャンネル名の最後に改行がついてないと、検索引っかからないため、一旦チャンネル名固定
	// 検索できるようになったら、コメントアウトのコードに変更する

	 std::cout << "<<" << ch_name << ">>" << std::endl;
	//  std::cout << "<<" << ch->GetName() << ">>" << std::endl;
	//テスト出力

	// チャンネルが存在しない場合
	if(!ch){
		std::cout << "!ch" << std::endl;
		msg_to_c = ERR_NOSUCHCHANNEL(client.GetNickname());
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
		// doesn't exist
	}
	//クライアントがチャンネルに参加していない場合
	else if(ch->GetPriv(client.GetUsername()) == P_None){
		std::cout << "<<test>>\n";
		msg_to_c = ERR_NOTONCHANNEL(client.GetNickname(), ch_name);
		SendMessage(client.GetFd(), msg_to_c, 0);
		return ;
		//you need oprator priv
	}
	//チャンネルのトピックを設定
	if(message.GetParams().size() == 2){
		topic = message.GetParams()[1];
		ch->SetToic(topic);
		msg_to_all = client.GetNickname() + "! TOPIC " + ch_name + " :"+ topic;
		ch->SendMsgToAll(msg_to_all);
	}
	// チャンネルのトピックを取得
	else{
		msg_to_all = client.GetNickname() + "! TOPIC " + ch_name + " :"+ ch->GetTopic() + "\n";
		ch->SendMsgToAll(msg_to_all);
	}
}