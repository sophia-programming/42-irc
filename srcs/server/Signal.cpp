#include "Server.hpp"

bool Server::Signal = false; //initialize static boolean
void Server::SignalHandler(int signum) {
	(void)signum;
	std::cout << GREEN << "Signal received!" << STOP << std::endl;
	Server::Signal = true;
}
