#include "Channel.hpp"

Channel::Channel(std::string channel_name):name_(channel_name), limit_(__LONG_MAX__),topic_("non"){
}

Channel::~Channel()
{
}

// ユーザーを一般権限でチャンネルに追加
// 1:Client& user ->　追加したいユーザーリファレンス
void Channel::AddUserAsN(Client& user)
{
    this->users_.insert(std::make_pair(&user,P_Nomal));
}

// ユーザーがオペレーターかどうかを確認する
// 1:const std::string &nickname -> 確認したいユーザーのニックネーム
bool Channel::IsOperator(const std::string &nickname) {
	Client* user = GetUser(nickname);
	if (user != NULL) {
		return users_.at(user) == P_Operator;
	}
	return false;
}

// ユーザーをオペレーター権限でチャンネルに追加
// 1:Client& user ->　追加したいユーザーリファレンス　
void Channel::AddUserAsO(Client& user)
{
    this->users_.insert(std::make_pair(&user, P_Operator));
}

// ユーザーをオペレーターから削除する
// 1:const std::string &nickname -> 削除したいユーザーのニックネーム
void Channel::RmUserFromOperator(const std::string &nickname) {
	Client* user = GetUser(nickname);
	if (user != NULL) {
		this->users_[user] = P_Nomal;
	}
}

// ユーザーを招待リストに追加
// 1:const std::string &nick_name -> 追加したいユーザーニックネーム
void Channel::AddUserinInvite(const std::string& nick_name)
{
    this->invate_users_.push_back(nick_name);
}

// ユーザーをチャンネルから削除
// 1:Client * user ->削除したいユーザーオブジェクト
void Channel::RmUser(Client * user)
{
    if(this->users_.size() < 1){
        return ;
    }
    this->users_.erase(user);

}

// ユーザーを招待リストから削除
// 1:const std::string &nick_name -> 削除したいユーザーニックネーム
void Channel::RmUserFromInvite(const std::string &nick_name)
{
    std::vector<std::string>::iterator iter;
    iter = std::find(this->invate_users_.begin(), this->invate_users_.end(), nick_name);
    if(iter != this->invate_users_.end()){
        this->invate_users_.erase(iter);
    }
}

// setter

void Channel::SetModeInvite(const bool invite)
{
    this->mode_.SetInvite(invite);
}

void Channel::SetModeTopic(const bool topic)
{
    this->mode_.SetTopic(topic);
}

void Channel::SetModeKey(const bool key)
{
    this->mode_.SetKey(key);
}

void Channel::SetModeLimit(long int user_limit)
{
    this->limit_ = user_limit;
}

// チャンネルのモードを設定する
void Channel::SetToic(const std::string& topic)
{
    this->topic_=topic;
}

void Channel::SetTopicSetter(Client* setter)
{
    this->topic_setter_ = setter;
}

void Channel::SetTopicTime(std::time_t time)
{
    this->topic_time_ = time;
}

void Channel::SetKey(const std::string& key)
{
    this->key_ = key;
}

void Channel::SetModeIsLimit(const bool is_limited)
{
    this->mode_.SetIsLimit(is_limited);
}

void Channel::SetLimit(long int limit)
{
    this->limit_ = limit;
}


// ユーザーの権限をオペレーターに設定する
// 1:const std::string &nick_name -> オペレーターにしたいユーザーのニックネーム
void Channel::SetPrivAsOperator(const std::string &nick_name)
{
    user_list_iter iter = this->users_.find(this->GetUser(nick_name));
    if(iter != this->users_.end()){
        iter->second = P_Operator;
    }
}

void Channel::SetPrivAsNomal(const std::string &nick_name)
{
    user_list_iter iter = this->users_.find(this->GetUser(nick_name));
    if(iter != this->users_.end()){
        iter->second = P_Nomal;
    }
}

//getter
const std::string &Channel::GetName() const
{
    return this->name_;
}

const std::string &Channel::GetTopic() const
{
    return this->topic_;
}

const Client *Channel::GetTopicSetter() const
{
    return this->topic_setter_;
}

const std::time_t Channel::GetTopicTime() const
{
    return this->topic_time_;
}

const std::string &Channel::GetKey() const
{
    return this->key_;
}

int Channel::GetLimit() const
{
    return this->limit_;
}

// 引数のモードが現在設定されているかどうか確認する関数
// 1:ChannelMode mode -> 確認したいモード
// bool Channel::CheckMode(ChannelMode mode)
// {
//     std::vector<ChannelMode>::iterator iter = std::find(this->mode_.begin(), this->mode_.end(), mode);
//     if(iter != this->mode_.end()){
//         return true;
//     }
//     return false;
// }

// getter for mode

bool Channel::GetModeInvite() const
{
    return this->mode_.GetInvite();
}

bool Channel::GetModeTopic() const
{
    return this->mode_.GetTopic();
}

bool Channel::GetModeKey() const
{
    return this->mode_.GetKey();
}

bool Channel::GetModeIsLimit() const
{
    return this->mode_.SetIsLimit();
}

long int Channel::GetModeLimit() const
{
    return this->mode_.GetLimit();
}

// 招待されているかの確認
// 1:std::string nick_name -> 確認したいユーザーのニックネーム
bool Channel::IsInvited(const std::string& nick_name){

    std::vector<std::string>::iterator iter;
    iter = find(this->invate_users_.begin(), this->invate_users_.end(), nick_name);
    if(iter != this->invate_users_.end()){
        return true;
    }
    return false;
}

// メッセージをこのチャンネルメンバー全員に送信する関数
// 1:const std::string& msg -> 送信したいメッセージ
void Channel::SendMsgToAll(const std::string& msg, Client* sender)
{
    user_list_iter iter = this->users_.begin();
    while(iter != this->users_.end()){
        if(iter->first != sender){
            send(iter->first->GetFd(), msg.c_str(), msg.size(), 0);
        }
        iter++;
    }
}

// ニックネームからClientオブジェクトを取得する関数
// 1:std::string nick_name ->取得したいユーザー名
Client* Channel::GetUser(const std::string& nick_name)
{
    std::map<Client*, User_Priv>::iterator iter = this->users_.begin();
    while(iter != this->users_.end()){
        if(iter->first->GetNickname() == nick_name){
            return iter->first;
        }
        iter++;
    }
    return NULL;
}

//指定したユーザーの権限を取得する
//1:std::string nick_name->権限を知りたいユーザーのニックネーム
const User_Priv Channel::GetPriv(const std::string& nick_name)
{
    Client* cl = this->GetUser(nick_name);
    std::cout << "nick_name: " << nick_name << std::endl;
    if(cl != NULL)
        return this->users_.find(cl)->second;
    throw ChannelException("Error: user dosent exist");
}

// チャンネルメンバーを取得する関数
// 1:const std::vector<Client*>& -> メンバーのリスト
const std::vector<Client*>& Channel::GetMember() const {
	return members_;
}

// mapの要素としてChannel classを扱うための比較演算子オーバーロード
bool Channel::operator<(const Channel &other) const
{
    return this->name_ < other.name_;
}

// チャンネルに指定したユーザーが存在するかどうかを確認する関数
bool Channel::HasUser(const std::string& nickname) const {
	for (std::map<Client*, User_Priv>::const_iterator it = users_.begin(); it != users_.end(); ++it) {
		if (it->first->GetNickname() == nickname) {
			return true;
		}
	}
	return false;
}

const char *Channel::ChannelException::what(void) const throw()
{
	return this->msg_.c_str();
}
