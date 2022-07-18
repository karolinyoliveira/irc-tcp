#ifndef utils_HPP
#define utils_HPP
#include "./socket.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

using namespace std;

bool isCommand(string str);
void execCommand(string cmd, int fileDescriptor);

/* Refer http://en.cppreference.com/w/c/program/signal */
void sigIntHandler(int sig_num);

#endif