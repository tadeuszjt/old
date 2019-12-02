#ifndef STRINGS_H
#define STRINGS_H

#include <string.h>

char *strAlloc(const char *chars);
char *strAllocChar(const char c);
void strPushChar(char **str, char c);
void strFree(char **str);

#endif
