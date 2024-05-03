#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "Color.hpp"

#define DEFAULT_PORT "6667"  // ポート番号は文字列として定義する必要がある

/*=== createSocket.cpp ===*/
int  initializeServer(const char* port, int &listenSocket);

/*=== handleConnections.cpp ===*/
void handleConnections(int listenSocket, std::string password);

#endif
