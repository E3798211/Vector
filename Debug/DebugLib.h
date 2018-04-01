#ifndef DEBUGLIB_H_INCLUDED
#define DEBUGLIB_H_INCLUDED

#include <iostream>

const char    BLUE[] = "\x1b[34m";
const char     RED[] = "\x1b[91m";
const char   GREEN[] = "\x1b[92m";
const char  YELLOW[] = "\x1b[93m";
const char  PURPLE[] = "\x1b[200m";
const char    CYAN[] = "\x1b[96m";
const char DEFAULT[] = "\x1b[0m";
const char MAGENTA[] = "\x1b[95m";

int PrintInfo(const char* msg);
int PrintErr (const char* msg);

#endif // DEBUGLIB_H_INCLUDED
