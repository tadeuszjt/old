#ifndef LEXEME_H
#define LEXEME_H

typedef enum {
	lexemeChar,
	lexemeWord,
	lexemeNumber,
} LexemeType;

typedef struct {
	LexemeType type;
	char       *value;
} Lexeme;

Lexeme getNextLexeme();
void   printLexeme(Lexeme lex);

#endif
