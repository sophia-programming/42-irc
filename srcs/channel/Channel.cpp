#include "Channel.hpp"

Channel::Channel(std::string channel_name):name_(channel_name), limit_(__LONG_MAX__),topic_("non"){
}

void Channel::AddUserAsN(Client& user)
{
    this->users_.emplace(user,P_Nomal);
}

void Channel::AddUserAsO(Client& user)
{
    this->users_.emplace(user, P_Operator);
}

void Channel::RmUser(const std::string& username)
{
    std::map<Client, User_Priv>::iterator iter = this->users_.begin();
    while(iter != this->users_.end()){
        if(iter->first.GetNickname() == username){
            this->users_.erase(iter);
            break;
        }
        iter++;
    }
}

// setter
void Channel::SetToic(const std::string& topic)
{
    this->topic_=topic;
}

void Channel::SetMode(const std::string &mode)
{
    this->mode_.emplace_back(mode);
}

void Channel::SetKey(const std::string &key)
{
    this->key_ = key;
}

void Channel::SetLimit(long int user_limit)
{
    this->limit_ = user_limit;
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

const std::string &Channel::GetKey() const
{
    return this->key_;
}

// 引数のモードが現在設定されているかどうか確認する関数
// 1:std::string& name -> 確認したいモード
bool Channel::CheckMode(ChannelMode mode)
{
    std::vector<ChannelMode>::iterator iter = std::find(this->mode_.begin(), this->mode_.end(), mode);
    if(iter != this->mode_.end()){
        return true;
    }
    return false;
}

//User名からClientオブジェクトを取得する関数
// 1: ->取得したいユーザー名
Client Channel::GetUser(std::string user_name)
{
    std::map<Client, User_Priv>::iterator iter = this->users_.begin();
    while(iter != this->users_.end()){
        if(iter->first.GetNickname() == user_name){
            return iter->first;
        }
        iter++;
    }
    throw ChannelException("user err");
}

//指定したユーザーの権限を取得する
//1:->権限を知りたいユーザー名
const User_Priv Channel::GetPriv(Client &user)
{
    return this->users_.find(user)->second;
}


const char *Channel::ChannelException::what(void) const throw()
{
	return this->msg_.c_str();
}