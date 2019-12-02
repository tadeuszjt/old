import Text.ParserCombinators.ReadP
import Control.Applicative
import Data.Char

data Number = NumInt Int | NumFloat Double deriving (Show)
data Val = ValNum Number | ValIdent String | ValExpr Expr deriving (Show)
data Fact = FactDivide Val Fact | FactVal Val deriving (Show)
data Term = TermTimes Fact Term | TermFact Fact deriving (Show)
data Expr = ExprPlus Term Expr | ExprMinus Term Expr | ExprTerm Term deriving (Show)

intLiteral :: ReadP String
intLiteral = do
    skipSpaces
    munch1 isDigit

floatLiteral :: ReadP String
floatLiteral = do
    skipSpaces
    a <++ b <++ c
    where
        a = do {pre <- munch1 isDigit; char '.'; post <- munch1 isDigit; return (pre ++ "." ++ post)}
        b = do {pre <- munch1 isDigit; char '.'; return (pre ++ "." ++ "0")}
        b = do {char '.'; post <- munch1 isDigit; return ("0" ++ "." ++ post)}

ident :: ReadP String
ident = do
    skipSpaces
    (:) <$> satisfy isLetter <*> munch (\x -> isLetter x || isDigit x)

sym c = do
    skipSpaces
    char c

numLiteral :: ReadP Number
numLiteral =
    (NumFloat . read <$> floatLiteral) <++
    (NumInt . read <$> intLiteral)

value :: ReadP Val
value =
    (ValExpr <$> between (sym '(') (sym ')') expr) <++
    (ValIdent <$> ident) <++
    (ValNum <$> numLiteral)

factor :: ReadP Fact
factor =
    do {v <- value; sym '/'; f <- factor; return $ FactDivide v f} <++
    (FactVal <$> value)

term :: ReadP Term
term =
    do {f <- factor; sym '*'; t <- term; return $ TermTimes f t} <++
    (TermFact <$> factor)

expr :: ReadP Expr
expr =
    do {t <- term; sym '+'; e <- expr; return $ ExprPlus t e} <++
    do {t <- term; sym '-'; e <- expr; return $ ExprMinus t e} <++
    (ExprTerm <$> term)

instance Num Number where
    (+) (NumInt a) (NumInt b) = NumInt (a + b)
    (+) a b                   = NumFloat (makeFloat a + makeFloat b)
    (-) (NumInt a) (NumInt b) = NumInt (a - b)
    (-) a b                   = NumFloat (makeFloat a - makeFloat b)
    (*) (NumInt a) (NumInt b) = NumInt (a * b)
    (*) a b                   = NumFloat (makeFloat a * makeFloat b)
    abs (NumInt x) = NumInt (abs x)
    abs (NumFloat x) = NumFloat (abs x)
    signum (NumInt x) = NumInt (signum x)
    signum (NumFloat x) = NumFloat (signum x)
    fromInteger x = NumInt (fromInteger x)

numDiv :: Number -> Number -> Number
numDiv a b = NumFloat (makeFloat a / makeFloat b)

makeFloat :: Number -> Double
makeFloat (NumInt n) = fromIntegral n
makeFloat (NumFloat n) = n

evalExpr :: Expr -> Number
evalExpr (ExprPlus t e) = (evalTerm t) + (evalExpr e)
evalExpr (ExprMinus t e) = (evalTerm t) - (evalExpr e)
evalExpr (ExprTerm t) = evalTerm t

evalTerm :: Term -> Number
evalTerm (TermTimes f t) = (evalFact f) * (evalTerm t) 
evalTerm (TermFact f) = (evalFact f)

evalFact :: Fact -> Number
evalFact (FactDivide v f) = (evalVal v) `numDiv` (evalFact f)
evalFact (FactVal v) = evalVal v

evalVal :: Val -> Number
evalVal (ValNum n) = n
evalVal (ValExpr e) = evalExpr e
evalVal (ValIdent "pi") = NumFloat pi

calculate :: String -> Number
calculate s = case readP_to_S expr s of
    ((e, ""):_) -> evalExpr e
    _ -> error "parse error"
