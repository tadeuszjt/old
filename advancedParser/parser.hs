import Data.Char
import Text.ParserCombinators.ReadP
import Control.Monad
import Control.Applicative

ident = do
    skipSpaces
    c <- satisfy isLetter
    s <- munch (\x -> isLetter x || isDigit x)
    return (c:s)

sym c = do
    skipSpaces
    char c

symTok c a = do
    skipSpaces
    sym c
    return a

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

keyword s = do
    id <- ident
    guard (id == s)
    return id

commaList p = do
    first <- p
    rest <- Text.ParserCombinators.ReadP.many (do sym ','; p)
    return (first:rest)

data Ordinal = OrdInt | OrdFloat deriving (Show)
data Typ = TypOrd Ordinal deriving (Show)

data Val = ValInt Int | ValFloat Float | ValIdent String deriving (Show)
data Op = Plus | Minus | Times | Divide deriving (Show)
data Expr = ExprLambAp LambAp | ExprPrefix Op Expr | ExprInfix Expr Op Expr | ExprVal Val deriving (Show)
data Pat = Pat [String] deriving (Show)
data Lamb = Lamb Pat Expr deriving (Show)
data LambAp = LambAp Lamb [Expr] deriving (Show)

value =
    intLit <|> floatLit <|> (ValIdent <$> ident)

pattern =
    Pat <$> commaList ident

lambda = do
    pat <- pattern
    str "->"
    exp <- expression
    return (Lamb pat exp)

lambAp = do
    sym '('
    lam <- lambda
    sym ')'
    exp <- Text.ParserCombinators.ReadP.many expression
    return $ LambAp lam exp

divide = symTok '/' Divide
times = symTok '*' Times
plus = symTok '+' Plus
minus = symTok '-' Minus

prefixy 2 = plus <|> minus
prefixy _ = pfail
infixy 2 = plus <|> minus
infixy 1 = times <|> divide
infixy _ = pfail

expression = expr 2
expr 0 =
    (ExprVal <$> value) <|>
    (do sym '('; e <- expr 2; sym ')'; return e) <|>
    (ExprLambAp <$> lambAp)
expr n =
    (expr (n-1) <|>
    do {op <- prefixy n; e <- expr n; return $ ExprPrefix op e} <|>
    do {en <- expr (n-1); op <- infixy n; e <- expr n; return $ ExprInfix en op e})

test =
    readP_to_S
