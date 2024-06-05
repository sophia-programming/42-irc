#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>


#include "Client.hpp"

class Client;

enum ChannelMode{
	CM_Non,
	CM_Invite, //  inviteされた人のみjoinを許可　チャンネルオペレーターだけinviteできる
	CM_Topic, // topicの書き換え権限がチャンオペのみ
	CM_Key, // キーワードを設定してわかる人のみjoin
	CM_Limit, //join可能人数を設定して制限する
	CM_Operator //オペレータ-権限のgive take
};

// Priviledge 権限
enum User_Priv{
	P_Nomal,
	P_Operator
};

class Channel{
	private:
		// チャンネル名
		std::string name_;
		// チャンネルトピック
		std::string topic_;
		// チャンネルモード
		std::vector<ChannelMode> mode_;
		// チャンネルキー（パスワード）
		std::string key_;
		// チャンネルの人数制限
		long int limit_;

		// 招待リスト
		std::vector<std::string> invate_users_;

		typedef std::map<Client*, User_Priv>::iterator user_list_iter;

	public:
		// ユーザーリスト
		std::map<Client*, User_Priv> users_;

	public:
		Channel(std::string channel_name);
		~Channel();

		// 一般ユーザーの追加
		void AddUserAsN(Client& user);
		// OPユーザーの追加
		void AddUserAsO(Client& user);
		void AddUserinInvite(const std::string& name);
		// ユーザーの削除
		void RmUser(Client *user);
		void RmUserFromInvite(const std::string& user);

		// setter
		void SetToic(const std::string& topic);
		// void SetMode(const std::string& mode);
		void SetKey(const std::string& key);
		void SetLimit(long int limit);

		// ユーザー権限をOPに設定
		void SetOperator(const std::string& user_name);

		// getter
		const std::string& GetName() const;
		const std::string& GetTopic() const;
		const std::string& GetKey() const;

		Client* GetUser(std::string user_name);
		const User_Priv GetPriv(std::string user_name);
		bool CheckMode(ChannelMode mode);
		void RmMode(ChannelMode mode);

		void SendMsgToAll(const std::string& msg);

		class ChannelException : public std::exception{
			private:
				std::string msg_;
			public:
				ChannelException(const std::string &msg):msg_(msg){};
				virtual ~ChannelException () throw() {};
				virtual const char* what (void) const throw();
		};

		bool operator<(const Channel& other)const;
};

#endif
