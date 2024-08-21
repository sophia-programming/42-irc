#ifndef MODE_HPP
#define MODE_HPP

#include <iostream>

class Mode
{
private:
	bool invite_; //inviteされた人のみjoinを許可 チャンネルオペレーターだけinviteできる
	bool topic_; //topicの書き換え権限がチャンオペのみ
	bool key_; //joinにkeyが必要
	bool is_limited_; //join可能人数を設定して制限する
	long int limit_; //join可能人数を設定して制限する
public:
	Mode();
	~Mode();
	//setter
	void SetInvite(bool invite);
	void SetTopic(bool topic);
	void SetKey(bool key);
	void SetIsLimit(bool is_limited);
	void SetLimit(long int limit);
	//getter
	bool GetInvite() const;
	bool GetTopic() const;
	bool GetKey() const;
	bool SetIsLimit() const;
	long int GetLimit() const;
};

#endif
