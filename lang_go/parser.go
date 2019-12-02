package main

import (
	"fmt"
)

var (
	tokStack  []Token
	tokIndex  int
)

func fillTokStack() {
	for {
		tok, ok := <-tokens
		if !ok {
			break
		}
		tokStack = append(tokStack, tok)
	}
}

func printTokStack() {
	for _, tok := range tokStack {
		fmt.Println(tok)
	}
}

func accept(t TokenType) bool {
	if tokStack[tokIndex].Type == t {
		tokIndex++
		return true
	}
	return false
}

func try(items... interface{}) bool {
	save := tokIndex
	for _, item := range items {
		switch v := item.(type) {
		case TokenType:
			if !accept(v) {
				tokIndex = save
				return false
			}

		case func() bool:
			if !v() {
				tokIndex = save
				return false
			}

		default:
			panic(item)
		}
	}
	return true
}

func empty() bool {
	return true
}

func sliceType() bool {
	return try(LBracket, RBracket)
}

func arrayType() bool {
	return try(LBracket, Number, RBracket)
}

func structAccess() bool {
	return try(Identifier, Period, structAccess) ||
		try(Identifier, Period, Identifier)
}

func structTypeField() bool {
	return try(define) || try(atype)
}

func structTypeFields() bool {
	return try(structTypeField, structTypeFields) || try(structTypeField)
}

func structType() bool {
	return try(LBrace, structTypeFields, RBrace)
}

func atype() bool {
	return try(Int) ||
		try(Float) ||
		try(Identifier) ||
		try(structType) ||
		try(sliceType, atype) ||
		try(arrayType, atype)
}

func define() bool {
	return try(Identifier, Colon, atype) || try(Identifier, Comma, define)
}

func arrayLiteralField() bool {
	return try(literal) || try(Identifier)
}

func arrayLiteralFields() bool {
	return try(arrayLiteralField, arrayLiteralFields) || try(arrayLiteralField)
}

func arrayLiteral() bool {
	return try(LBracket, arrayLiteralFields, RBracket)
}

func literal() bool {
	return try(Number) || try(arrayLiteral)
}

func pow() bool {
	return try(Times, Times)
}

func operator() bool {
	return try(pow) || try(Times) || try(Plus) || try(Minus)
}

func term() bool {
	return try(Number) || try(operator) || try(structAccess) || try(Identifier) || try(literal)
}

func expression() bool {
	return try(term, expression) || try(term)
}

func assign() bool {
	return try(Identifier, Colon, Equals, expression) || try(Identifier, Comma, assign)
}

func functionOperator() bool {
	return try(Colon, Colon)
}

func functionReturns() bool {
	return try(define, functionReturns) || try(define) || try(atype)
}

func functionParams() bool {
	return try(define, functionParams) || try(define)
}

func function() bool {
	return try(functionParams, Identifier, functionOperator, functionReturns)
}

func statement() bool {
	return try(function, NewLine) ||
		try(define, NewLine) ||
		try(assign, NewLine) ||
		try(expression, NewLine) ||
		try(Identifier, NewLine) ||
		try(NewLine) ||
		try(Tab, statement)
}

func program() bool {
	return try(EndOfTokens) || try(statement, program)
}

func parse() {
	fillTokStack()
	printTokStack()

	fmt.Println(program())

}
