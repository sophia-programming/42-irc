#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

int Client::GetFD() {
	return this->FD;
}

void Client::SetFD(int fd) {
	this->FD = fd;
}

void Client::SetIPAddress(std::string ipaddress) {
	this->IPaddress = ipaddress;
}
