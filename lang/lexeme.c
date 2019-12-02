#include <ctype.h>
#include <stdio.h>
#include "lexeme.h"
#include "strings.h"

char c = ' ';
void loadNextChar() {
	c = getchar();
}

char *buf = NULL;

Lexeme getNextLexeme() {
	Lexeme lex = {type: -1, value: NULL};

	while (c == ' ')
		loadNextChar();

	if (isalpha(c)) {
		while (isalpha(c) || isalnum(c)) {
			strPushChar(&buf, c);
			loadNextChar();
		}
		lex.type = lexemeWord;
		lex.value = strAlloc(buf);
	}
	else if (isalnum(c)) {
		while (isalnum(c)) {
			strPushChar(&buf, c);
			loadNextChar();
		}
		lex.type = lexemeNumber;
		lex.value = strAlloc(buf);
	}
	else {
		lex.type = lexemeChar;
		lex.value = strAllocChar(c);
		loadNextChar();
	}

	strFree(&buf);
	return lex;
}

void printLexeme(Lexeme lex) {
	switch (lex.type) {
	case lexemeChar:
		printf("lexemeChar  : %s\n", lex.value);
		break;
	case lexemeWord:
		printf("lexemeWord  : %s\n", lex.value);
		break;
	case lexemeNumber:
		printf("lexemeNumber: %s\n", lex.value);
		break;
	default:
		printf("lexeme<unknown>: %s\n", lex.value);
		break;
	}
}
