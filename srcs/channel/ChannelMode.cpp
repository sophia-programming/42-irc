#include "Mode.hpp"

Mode::Mode():invite_(false), topic_(false), key_(false), limit_(0)
{
}

Mode::~Mode()
{
}

void Mode::SetInvite(bool invite)
{
	this->invite_ = invite;
}

void Mode::SetTopic(bool topic)
{
	this->topic_ = topic;
}

void Mode::SetKey(bool key)
{
	this->key_ = key;
}

void Mode::SetIsLimit(bool is_limited)
{
	this->is_limited_ = is_limited;
}

void Mode::SetLimit(long int limit)
{
	this->limit_ = limit;
}

bool Mode::GetInvite() const
{
	return this->invite_;
}

bool Mode::GetTopic() const
{
	return this->topic_;
}

bool Mode::GetKey() const
{
	return this->key_;
}

bool Mode::SetIsLimit() const
{
	return this->is_limited_;
}

long int Mode::GetLimit() const
{
	return this->limit_;
}