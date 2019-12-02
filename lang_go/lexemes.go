package main

import "unicode"

func init() {
	go getLexemes()
}

var lexemes = make(chan Lexeme)

type LexemeType int

const (
	lexemeChar LexemeType = iota
	lexemeWord
	lexemeNumber
)

func (l LexemeType) String() string {
	return map[LexemeType]string{
		lexemeChar:   "lexemeChar",
		lexemeWord:   "lexemeWord",
		lexemeNumber: "lexemeNumber",
	}[l]
}

type Lexeme struct {
	Type  LexemeType
	Value string
}

const (
	modeChar = iota
	modeWord
	modeNumber
)

var mode = modeChar

func getLexemes() {
	str := ""

	for {
		r, ok := <-runes
		if !ok {
			break
		}

		for {
			switch mode {
			case modeChar:
				if unicode.IsLetter(r) {
					mode = modeWord
					continue
				} else if unicode.IsDigit(r) {
					mode = modeNumber
					continue
				} else if unicode.IsSpace(r) {
					if r == '\n' {
						lexemes <- Lexeme{lexemeChar, string(r)}
					} else if r == '\t' {
						lexemes <- Lexeme{lexemeChar, string(r)}
					}
				} else {
					lexemes <- Lexeme{lexemeChar, string(r)}
				}

			case modeWord:
				if unicode.IsLetter(r) || unicode.IsDigit(r) {
					str += string(r)
				} else {
					lexemes <- Lexeme{lexemeWord, str}
					str = ""
					mode = modeChar
					continue
				}

			case modeNumber:
				if unicode.IsDigit(r) {
					str += string(r)
				} else {
					lexemes <- Lexeme{lexemeNumber, str}
					str = ""
					mode = modeChar
					continue
				}
			}

			break
		}
	}

	close(lexemes)
}
