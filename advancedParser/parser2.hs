import Data.Char
import Text.ParserCombinators.ReadP
import Control.Monad
import Control.Applicative

data Ordinal = OrdInt | OrdFloat | OrdBool deriving (Show)
data Typ = TypOrd Ordinal | TypStruct [Typ] | TypArr Typ deriving (Show)

data Val =
    ValIdent String |
    ValInt Int |
    ValFloat Float |
    ValBool Bool |
    ValStruct [Val] |
    ValArr Typ [Val]
    deriving (Show)

ident = do
    skipSpaces
    c <- satisfy isLetter
    s <- munch (\x -> isLetter x || isDigit x)
    return (c:s)

sym c = do
    skipSpaces
    char c

str s = do
    skipSpaces
    string s

intLit = do
    skipSpaces
    ValInt . read <$> munch1 isDigit

floatLit = do
    skipSpaces
    ValFloat . read <$> floatLit'
floatLit' =
    do {pre <- munch1 isDigit; char '.'; post <- munch1 isDigit; return (pre ++ "." ++ post)} <++
    do {char '.'; post <- munch1 isDigit; return ("0." ++ post)} <++
    do {pre <- munch1 isDigit; char '.'; return (pre ++ ".0")}

boolLit =
    do {keyword "false"; return $ ValBool False} <|> 
    do {keyword "true"; return $ ValBool True}
    
keyword s = do
    id <- ident
    guard (id == s)
    return id

typOrd =
    do {keyword "int"; return OrdInt} <|>
    do {keyword "float"; return OrdFloat} <|>
    do {keyword "bool"; return OrdBool}

commaList p = do
    first <- p
    rest <- Text.ParserCombinators.ReadP.many (do sym ','; p)
    return (first:rest)
    
typ = 
    do {o <- typOrd; return $ TypOrd o} <|>
    do {sym '{'; fields <- commaList typ; sym '}'; return $ TypStruct fields} <|>
    do {sym '['; t <- typ; sym ']'; return $ TypArr t}

value =
    intLit <|> floatLit <|> boolLit <|> structLit <|> (ValIdent <$> ident)

data Func = FuncIdent String deriving (Show)

funcCall =
    
    
structLit =
    do sym '{'; vals <- commaList value; sym '}'; return (ValStruct vals)

test p =
    readP_to_S p 
