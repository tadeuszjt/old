import Text.ParserCombinators.ReadP
import Control.Applicative
import Data.Char

data Number = NumInt Int | NumFloat Double deriving (Show)
data Val = ValNum Number | ValIdent String | ValExpr Expr | ValApply Lamb [Val] deriving (Show)
data Fact = FactDivide Val Fact | FactVal Val deriving (Show)
data Term = TermTimes Fact Term | TermFact Fact deriving (Show)
data Expr = ExprPlus Term Expr | ExprMinus Term Expr | ExprTerm Term deriving (Show)
data Lamb = Lamb [String] Expr deriving (Show)

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
        c = do {char '.'; post <- munch1 isDigit; return ("0" ++ "." ++ post)}

ident :: ReadP String
ident = do
    skipSpaces
    (:) <$> satisfy isLetter <*> munch (\x -> isLetter x || isDigit x)

sym c = do
    skipSpaces
    char c

str s = do
    skipSpaces
    string s

numLiteral :: ReadP Number
numLiteral =
    (NumFloat . read <$> floatLiteral) <++
    (NumInt . read <$> intLiteral)

value :: ReadP Val
value =
    do {lam@(Lamb args exp) <- lambda; vals <- count (length args) value; return (ValApply lam vals)} <++
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

lambda :: ReadP Lamb
lambda = 
    do sym '('; args <- many1 ident; str "->"; exp <- expr; sym ')'; return (Lamb args exp)

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

type SymbolTable = [(String, Number)]

evalExpr :: SymbolTable -> Expr -> Number
evalExpr st (ExprPlus t e) = (evalTerm st t) + (evalExpr st e)
evalExpr st (ExprMinus t e) = (evalTerm st t) - (evalExpr st e)
evalExpr st (ExprTerm t) = evalTerm st t

evalTerm :: SymbolTable -> Term -> Number
evalTerm st (TermTimes f t) = (evalFact st f) * (evalTerm st t) 
evalTerm st (TermFact f) = (evalFact st f)
    
evalFact :: SymbolTable -> Fact -> Number
evalFact st (FactDivide v f) = (evalVal st v) `numDiv` (evalFact st f)
evalFact st (FactVal v) = evalVal st v

evalVal :: SymbolTable -> Val -> Number
evalVal st val = case val of
    ValApply (Lamb args exp) vals -> evalExpr ((zip args $ map (evalVal st) vals) ++ st) exp
    ValNum n       -> n
    ValExpr e      -> evalExpr st e
    ValIdent ident -> case filter (\(str, num) -> str == ident) st of
        ((s, n):_) -> n
        _          -> error ("couldn't resolve ident: " ++ ident)

calculate :: String -> Number
calculate s = case readP_to_S expr s of
    ((e, ""):_) -> evalExpr [("pi", NumFloat pi)] e
    _ -> error "parse error"
