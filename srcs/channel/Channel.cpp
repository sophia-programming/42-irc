#include "Channel.hpp"

Channel::Channel(std::string channel_name):name_(channel_name), limit_(__LONG_MAX__),topic_("non"){
}

Channel::~Channel()
{
}

void Channel::AddUserAsN(Client& user)
{
    this->users_.insert(std::make_pair(&user,P_Nomal));
}

void Channel::AddUserAsO(Client& user)
{
    this->users_.insert(std::make_pair(&user, P_Operator));
}

void Channel::AddUserinInvite(const std::string& user_name)
{
    this->invate_users_.push_back(user_name);
}

void Channel::RmUser(Client * user)
{
    if(this->users_.size() < 1){
        return ;
    }
    this->users_.erase(user);

}

void Channel::RmUserFromInvite(const std::string &user_name)
{
    std::vector<std::string>::iterator iter;
    iter = std::find(this->invate_users_.begin(), this->invate_users_.end(), user_name);
    if(iter != this->invate_users_.end()){
        this->invate_users_.erase(iter);
    }
}

// setter
void Channel::SetToic(const std::string& topic)
{
    this->topic_=topic;
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

void Channel::SendMsgToAll(const std::string& msg)
{
    user_list_iter iter = this->users_.begin();
    while(iter != this->users_.end()){
        send(iter->first->GetFd(), msg.c_str(), msg.size(), 0);
        iter++;
    }
}

bool Channel::operator<(const Channel &other) const
{
    return this->name_ < other.name_;
}

//User名からClientオブジェクトを取得する関数
// 1: ->取得したいユーザー名
Client* Channel::GetUser(std::string user_name)
{
    std::map<Client*, User_Priv>::iterator iter = this->users_.begin();
    while(iter != this->users_.end()){
        if(iter->first->GetNickname() == user_name){
            return iter->first;
        }
        iter++;
    }
    return NULL;
}

//指定したユーザーの権限を取得する
//1:->権限を知りたいユーザー名
const User_Priv Channel::GetPriv(std::string user_name)
{
    Client* cl = this->GetUser(user_name);
    if(cl != NULL)
        return this->users_.find(cl)->second;
    throw ChannelException("Erroe: user dosent exist");
}

const char *Channel::ChannelException::what(void) const throw()
{
	return this->msg_.c_str();
}
