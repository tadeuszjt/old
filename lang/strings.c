#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"

char *strAlloc(const char *s) {
	char *str = malloc((strlen(s)+1) *sizeof(char));
	strcpy(str, s);
	return str;
}

char *strAllocChar(const char c) {
	char *str = malloc(2 * sizeof(char));
	if (str == NULL) {
		fputs("strAllocChar: malloc failed\n", stderr);
		return NULL;
	}

	str[0] = c;
	str[1] = '\0';
	return str;
}

void strPushChar(char **str, char c) {
	if (*str == NULL) {
		*str = strAllocChar(c);
		return;
	}

	int len = strlen(*str);

	*str = realloc(*str, (len+2)*sizeof(char));
	if (*str == NULL) {
		fputs("strPushChar: realloc failed\n", stderr);
		return;
	}

	(*str)[len] = c;
	(*str)[len+1] = '\0';
}

void strFree(char **str) {
	if (*str != NULL) {
		free(*str);
		*str = NULL;
	}
}


