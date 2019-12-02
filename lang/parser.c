#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "token.h"
#include "strings.h"
#include "node.h"

struct {
	int   len, cap;
	Token *stack;
} tokens = {0, 0, NULL};
void pushToken(Token tok);
void loadTokens();
void freeTokens();
void printTokens();

Token *next = NULL;

typedef enum {
	success = 1,
	fail = 0,
} Ret;

Ret statement() {
}

Ret program() {
	Token *save = next;
}

void run() {
	loadTokens();
	printTokens();

	program();

	freeTokens();
}

void pushToken(Token tok) {
	if (tokens.len == tokens.cap) {
		if (tokens.cap == 0) {
			tokens.cap = 8;
		} else {
			tokens.cap *= 2;
		}
		tokens.stack = realloc(tokens.stack, tokens.cap * sizeof(Token));
	}
	tokens.stack[tokens.len++] = tok;
}

void loadTokens() {
	for (;;) {
		Token t = getNextToken();
		pushToken(t);
		if (t.type == EndOfTokens)
			break;
	}

	next = tokens.stack;
}

void freeTokens() {
	for (int i = 0; i < tokens.len; i++)
		strFree(&(tokens.stack[i].value));

	free(tokens.stack);
}

void printTokens() {
	for (int i = 0; i < tokens.len; i++) {
		printToken(tokens.stack[i]);
	}
}
