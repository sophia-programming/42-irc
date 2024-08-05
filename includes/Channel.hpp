#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Client.hpp"
#include "Mode.hpp"

class Client;

// enum ChannelMode{

// 	CM_Invite, //  inviteされた人のみjoinを許可　チャンネルオペレーターだけinviteできる
// 	CM_Topic, // topicの書き換え権限がチャンオペのみ
// 	CM_Key, // キーワードを設定してわかる人のみjoin
// 	CM_Limit, //join可能人数を設定して制限する
// 	CM_Operator //オペレータ-権限のgive take
// };

// Priviledge 権限
enum User_Priv{
	P_None, // 権限なし
	P_Nomal, // 一般ユーザー
	P_Operator //オペレーター権限ユーザー
};
class Channel{
	private:
		// チャンネル名
		std::string name_;
		// チャンネルトピック
		std::string topic_;
		// トピック設定者
		Client* topic_setter_;
		// トピック設定日時
		std::time_t topic_time_;
		// チャンネルモード
		// std::vector<ChannelMode> mode_;
		Mode mode_;
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

		void RmUser(Client *user);
		void RmUserFromInvite(const std::string& user);
		void RmUserFromOperator(const std::string& user);

		// setter
		void SetToic(const std::string& topic);
		void SetTopicSetter(Client* setter);
		void SetTopicTime(std::time_t time);
		void SetKey(const std::string& key);
		void SetLimit(long int limit);

		// setter for mode
		void SetModeInvite(const bool invite);
		void SetModeTopic(const bool topic);
		void SetModeKey(const bool key);
		void SetModeIsLimit(const bool is_limited);
		void SetModeLimit(long int limit);

		void SetPrivAsOperator(const std::string& nick_name);
		void SetPrivAsNomal(const std::string& nick_name);

		// getter
		const std::string& GetName() const;
		const std::string& GetTopic() const;
		const Client* GetTopicSetter() const;
		const std::time_t GetTopicTime() const;
		const std::string& GetKey() const;
		int GetLimit() const;
		const std::vector<Client*>& GetMember() const;
		// getter for mode
		bool GetModeInvite() const;
		bool GetModeTopic() const;
		bool GetModeKey() const;
		bool GetModeIsLimit() const;
		long int GetModeLimit() const;

		Client* GetUser(const std::string& nick_name);
		const User_Priv GetPriv(const std::string& nick_name);
		// bool CheckMode(ChannelMode mode);
		bool IsInvited(const std::string& nick_name);
		bool IsOperator(const std::string &nickname);


	void SendMsgToAll(const std::string& msg, Client* sender);

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
