import Control.Monad
import Data.List
import Data.Char
import Data.Maybe

data Token = Let | Equals | Semicolon | Plus | Minus | Times | Divide | LParen | RParen | Ident String | Number Int deriving (Show, Eq)

tokens = [
    ("let", Let),
    ("=", Equals),
    (";", Semicolon),
    ("+", Plus),
    ("-", Minus),
    ("*", Times),
    ("/", Divide),
    ("(", LParen),
    (")", RParen)
    ]

lexer :: String -> [Token]
lexer "" = []
lexer xs
    | not $ null matches = let (str, tok) = head matches in tok : (lexer $ drop (length str) xs) 
    where matches = filter (\(k, v) -> isPrefixOf k xs) tokens
lexer all@(x:xs)
    | isSpace x  = lexer xs
    | isDigit x  = Number (read $ takeWhile isDigit all) : lexer (dropWhile isDigit all)
    | isLetter x = Ident (takeWhile isLetter all) : lexer (dropWhile isLetter all)
    | otherwise  = error ("lex error at: " ++ (take 20 all) ++ "...")

data AdOp = AdOpPlus | AdOpMinus deriving (Show)
data Val  = ValIdent String | ValNumber Int | ValExpr Expr deriving (Show)
data Fact = FactDivide Val Fact | FactVal Val deriving (Show)
data Term = TermTimes Fact Term | TermFact Fact deriving (Show)
data Expr = ExprAdOp Term AdOp Expr | ExprTerm Term deriving (Show)
data Node = NodeExpr Expr | NodeTerm Term | NodeFact Fact | NodeToken Token | NodeAdOp AdOp | NodeVal Val deriving (Show)

type ParserFunc = [Token] -> Maybe (Node, [Token])

try :: [[ParserFunc]] -> [Token] -> ([Node], [Token]) 
try []       _  = ([], [])
try ([]:pss) ts = try pss ts
try ([p]:pss) ts = case p ts of
    Nothing -> try pss ts
    Just (n, rest) -> ([n], rest)
try ((p:ps):pss) ts = case try [[p]] ts of
    ([], []) -> try pss ts
    ([n], rest) -> (case try [ps] rest of
        ([], []) -> try pss ts
        (ns, rest') -> (n:ns, rest'))

-- AdOp -> + | -
-- V    -> I | N | ( E )
-- F    -> V / F | V
-- T    -> F * T | F
-- E    -> T AdOp E | T

parseToken tok [] = Nothing
parseToken tok (t:ts) = if t == tok
    then Just (NodeToken tok, ts)
    else Nothing

parseIdent (Ident id:ts) = Just (NodeToken $ Ident id, ts)
parseIdent _ = Nothing

parseNumber (Number num:ts) = Just (NodeToken $ Number num, ts)
parseNumber _ = Nothing

parseAdOp (Plus:ts) = Just (NodeAdOp AdOpPlus, ts)
parseAdOp (Minus:ts) = Just (NodeAdOp AdOpMinus, ts)
parseAdOp _ = Nothing

parseVal :: ParserFunc
parseVal ts = case try [[parseIdent], [parseNumber], [parseToken LParen, parseExpr, parseToken RParen]] ts of
    ([NodeToken (Ident id)], rest)   -> Just (NodeVal $ ValIdent id, rest)
    ([NodeToken (Number num)], rest) -> Just (NodeVal $ ValNumber num, rest)
    ([_, NodeExpr expr, _], rest)    -> Just (NodeVal $ ValExpr expr, rest)
    _ -> Nothing

parseFact :: ParserFunc
parseFact ts = case try [[parseVal, parseToken Divide, parseFact], [parseVal]] ts of
    ([NodeVal val, _, NodeFact fact], rest) -> Just (NodeFact $ FactDivide val fact, rest)
    ([NodeVal val], rest)                   -> Just (NodeFact $ FactVal val, rest)
    _ -> Nothing

parseTerm :: ParserFunc
parseTerm ts = case try [[parseFact, parseToken Times, parseTerm], [parseFact]] ts of
    ([NodeFact fact, _, NodeTerm term], rest) -> Just (NodeTerm $ TermTimes fact term, rest)
    ([NodeFact fact], rest)                   -> Just (NodeTerm $ TermFact fact, rest)
    _ -> Nothing

parseExpr :: ParserFunc
parseExpr ts = case try [[parseTerm, parseAdOp, parseExpr], [parseTerm]] ts of
    ([NodeTerm term, NodeAdOp adop, NodeExpr expr], rest) -> Just (NodeExpr $ ExprAdOp term adop expr, rest)
    ([NodeTerm term], rest)                               -> Just (NodeExpr $ ExprTerm term, rest)
    _ -> Nothing

evalVal val = case val of
    ValNumber num -> num
    ValExpr expr  -> evalExpr expr
evalFact fact = case fact of 
    FactDivide val fact -> (evalVal val) `div` (evalFact fact)
    FactVal val         -> evalVal val
evalTerm term = case term of
    TermTimes fact term -> (evalFact fact) * (evalTerm term)
    TermFact fact       -> evalFact fact
evalExpr expr = case expr of
    ExprAdOp term AdOpPlus expr  -> (evalTerm term) + (evalExpr expr)
    ExprAdOp term AdOpMinus expr -> (evalTerm term) - (evalExpr expr)
    ExprTerm term                -> evalTerm term

main = forever $ do
    l <- getLine
    print $ case parseExpr $ lexer l of
        Nothing -> "invalid"
        Just (NodeExpr expr, _) -> show $ evalExpr expr
