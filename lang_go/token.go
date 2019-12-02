package main

type TokenType int

const (
	Invalid TokenType = iota
	Identifier
	Number
	Colon
	Comma
	Period
	LBracket
	RBracket
	LBrace
	RBrace
	Plus
	Minus
	Equals
	Times
	Int
	Float
	NewLine
	Tab
	EndOfTokens
)

func (t TokenType) String() string {
	return map[TokenType]string{
		Identifier: "Identifier",
		Number:     "Number",
		Colon:      "Colon",
		Comma:      "Comma",
		Period:     "Period",
		LBracket:   "LBracket",
		RBracket:   "RBracket",
		LBrace:     "LBrace",
		RBrace:     "RBrace",
		Plus:       "Plus",
		Minus:      "Minus",
		Equals:     "Equals",
		Times:      "Times",
		Int:        "Int",
		Float:      "Float",
		NewLine:    "NewLine",
		Tab:        "Tab",
		Invalid:    "Invalid",
		EndOfTokens: "EndOfTokens",
	}[t]
}

var mapChar = map[string]TokenType{
	":":  Colon,
	",":  Comma,
	".":  Period,
	"[":  LBracket,
	"]":  RBracket,
	"{":  LBrace,
	"}":  RBrace,
	"+":  Plus,
	"-":  Minus,
	"=":  Equals,
	"*":  Times,
	"\n": NewLine,
	"\t": Tab,
}

var mapWord = map[string]TokenType{
	"int":   Int,
	"float": Float,
}

type Token struct {
	Type  TokenType
	Value string
}

func init() {
	go getTokens()
}

var tokens = make(chan Token)

func getTokens() {
	for {
		lex, ok := <-lexemes
		if !ok {
			break
		}

		switch lex.Type {
		case lexemeChar:
			if tokType, ok := mapChar[lex.Value]; ok {
				tokens <- Token{Type: tokType}
			} else {
				tokens <- Token{Type: Invalid, Value: lex.Value}
			}

		case lexemeWord:
			if tokType, ok := mapWord[lex.Value]; ok {
				tokens <- Token{Type: tokType}
			} else {
				tokens <- Token{Type: Identifier, Value: lex.Value}
			}

		case lexemeNumber:
			tokens <- Token{
				Type:  Number,
				Value: lex.Value,
			}
		}
	}
	tokens <- Token{ Type:  EndOfTokens }

	close(tokens)
}
