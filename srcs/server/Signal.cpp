#include "Server.hpp"

bool Server::signal = false; //initialize static boolean
void Server::SignalHandler(int signum) {
	(void)signum;
	std::cout << GREEN << "signal received!" << STOP << std::endl;
	Server::signal = true;
}
