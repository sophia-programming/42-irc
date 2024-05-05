#include <iostream>
#include "Color.hpp"

#include <stdlib.h>
//__attribute__((destructor))
//static void destructor() {
//	system("leaks -q ircserv");
//}

int main(int argc, char **argv){
	(void)argc;
	(void)argv;
	std::cout << GREEN << "ft_irc start!" << STOP << std::endl;
	return 0;
}
