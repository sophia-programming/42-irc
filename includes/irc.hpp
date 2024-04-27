#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define DEFAULT_PORT "6667"  // ポート番号は文字列として定義する必要がある

/*=== createSocket.cpp ===*/
int  initializeServer(const char* port, int &listenSocket);

/*=== handleConnections.cpp ===*/
void handleConnections(int listenSocket, std::string password);

/* colors */
const char *const STOP = "\033[0m";
const char *const BOLD = "\033[1m";
const char *const BLACK = "\033[30m";
const char *const RED = "\033[31m";
const char *const GREEN = "\033[32m";
const char *const YELLOW = "\033[33m";
const char *const BLUE = "\033[34m";
const char *const MAGENTA = "\033[35m";
const char *const CYAN = "\033[36m";
const char *const WHITE = "\033[37m";
const char *const UNDERLINE =" \033[4m";

#endif
