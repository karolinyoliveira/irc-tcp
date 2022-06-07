#ifndef utils_HPP
#define utils_HPP
#include "./socket.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h> 

using namespace std;

bool isCommand(string str);
void execCommand(string cmd);

/* used to accepted more than 4095 chars from stdin
 REF:https://stackoverflow.com/questions/39546500/how-to-make-scanf-to-read-more-than-4095-characters-given-as-input */
int clear_icanon(void);

#endif