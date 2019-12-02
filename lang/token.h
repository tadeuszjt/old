#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
	Plus,
	Identifier,
	Number,
	Period,
	Semicolon,
	Colon,
	Equals,
	Comma,
	LBracket,
	RBracket,
	Int,
	Float,
	Newline,
	Tab,
	EndOfTokens,
} TokenType;

typedef struct {
	TokenType type;
	char      *value;
} Token;

Token getNextToken();
void  printToken();


#endif
