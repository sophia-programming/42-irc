#include "Client.hpp"

Client::Client() {
	std::cout << "Client constructor" << std::endl;
}

Client::~Client() {
	std::cout << "Client destructor" << std::endl;
}

int Client::GetFD() {
	return this->FD;
}

void Client::SetFD(int fd) {
	this->FD = fd;
}

void Client::SetIPAddress(std::string ipaddress) {
	this->IPaddress = ipaddress;
}
