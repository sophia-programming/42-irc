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
	P_Nomal, // 一般ユーザー
	P_Operator //オペレーター権限ユーザー
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

		// 招待リスト（nick_nameの配列）
		std::vector<std::string> invate_users_;

		// チャンネルメンバー
		std::vector<Client*> members_;

		typedef std::map<Client*, User_Priv>::iterator user_list_iter;

	public:
		// ユーザーリスト（clientポインタと、そのユーザーの権限）
		std::map<Client*, User_Priv> users_;

	public:
		Channel(std::string channel_name);
		~Channel();

		void AddUserAsN(Client& user);
		void AddUserAsO(Client& user);
		void AddUserinInvite(const std::string& name);
		bool IsMember(const Client* client) const;

		void RmUser(Client *user);
		void RmUserFromInvite(const std::string& user);
		void RmUserFromOperator(const std::string& user);

		// setter
		void SetToic(const std::string& topic);
		void SetKey(const std::string& key);
		void SetLimit(long int limit);
		// void SetMode(const std::string& mode);

		void SetPrivAsOperator(const std::string& nick_name);
		void SetPrivAsNomal(const std::string& nick_name);

		// getter
		const std::string& GetName() const;
		const std::string& GetTopic() const;
		const std::string& GetKey() const;
		int GetLimit() const;
		const std::vector<Client*>& GetMember() const;


		Client* GetUser(const std::string& nick_name);
		const User_Priv GetPriv(const std::string& nick_name);
		bool CheckMode(ChannelMode mode);
		bool IsInvited(const std::string& nick_name);
		bool IsOperator(const std::string &nickname);


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
