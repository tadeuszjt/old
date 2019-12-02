#include <stdio.h>
#include "token.h"
#include "lexeme.h"
#include "strings.h"

Token getNextToken() {
	Lexeme lex = getNextLexeme();
	Token tok = {type: EndOfTokens, value: NULL};

	switch (lex.type) {
	case lexemeChar:
		switch (lex.value[0]) {
		case '+':
			tok.type = Plus;
			break;
		case ':':
			tok.type = Colon;
			break;
		case ';':
			tok.type = Semicolon;
			break;
		case '=':
			tok.type = Equals;
			break;
		case '.':
			tok.type = Period;
			break;
		case ',':
			tok.type = Comma;
			break;
		case '[':
			tok.type = LBracket;
			break;
		case ']':
			tok.type = RBracket;
			break;
		case '\n':
			tok.type = Newline;
			break;
		case '\t':
			tok.type = Tab;
			break;
		case EOF:
			tok.type = EndOfTokens;
			break;
		default:
			fprintf(stderr, "token.c: lexeme char invalid\n");
			tok.type = EndOfTokens;
			break;
		}
		break;

	case lexemeWord:
		if (strcmp(lex.value, "int") == 0) {
			tok.type = Int;
		} else if (strcmp(lex.value, "float") == 0) {
			tok.type = Float;
		} else {
			tok.type = Identifier;
			tok.value = strAlloc(lex.value);
		}
		break;

	case lexemeNumber:
		tok.type = Number;
		tok.value = strAlloc(lex.value);
		break;

	default:
		fprintf(stderr, "token.c: tok.type invalid\n");
		break;
	}

	strFree(&lex.value);
	return tok;
}

void printToken(Token tok) {
	switch (tok.type) {
	case Plus:
		printf("Plus\n");
		break;
	case Semicolon:
		printf("Semicolon\n");
		break;
	case Colon:
		printf("Colon\n");
		break;
	case Equals:
		printf("Equals\n");
		break;
	case Period:
		printf("Period\n");
		break;
	case Comma:
		printf("Comma\n");
		break;
	case RBracket:
		printf("RBracket\n");
		break;
	case LBracket:
		printf("LBracket\n");
		break;
	case Identifier:
		printf("Identifier: %s\n", tok.value);
		break;
	case Number:
		printf("Number    : %s\n", tok.value);
		break;
	case Int:
		printf("Int\n");
		break;
	case Float:
		printf("Float\n");
		break;
	case Newline:
		printf("Newline\n");
		break;
	case Tab:
		printf("Tab\n");
		break;
	case EndOfTokens:
		printf("EndOfTokens\n");
		break;
	default:
		printf("token<Unknown> : %s\n", tok.value);
		break;
	}
}


