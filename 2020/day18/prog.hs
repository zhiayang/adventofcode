-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Data.Char
import Debug.Trace

data Expr = Number Int | Add Expr Expr | Multiply Expr Expr deriving (Show, Eq)

parseNumber :: String -> Maybe (Expr, String)
parseNumber xs = Just (let x = (Number $ int $ takeWhile isDigit xs, dropWhile isDigit xs) in  x)

parseFactor :: (String -> Maybe (Expr, String)) -> String -> Maybe (Expr, String)
parseFactor pe (' ':xs) = parseFactor pe xs
parseFactor pe (x:xs) = if x == '('
    then case pe xs of
        Just (r, (y:ys)) -> Just (r, ys)
        Nothing          -> Nothing
    else parseNumber (x:xs)

parseTerm :: String -> Maybe (Expr, String)
parseTerm inp = parseFactor parseExpr inp
    >>= (\(f, xs) -> case dropWhile (== ' ') xs of
            ('+':xs') -> (parseTerm xs') <&> (\(r, ys) -> (Add f r, ys))
            _         -> Just (f, xs))

parseExpr :: String -> Maybe (Expr, String)
parseExpr inp = parseTerm inp
    >>= (\(f, xs) -> case dropWhile (== ' ') xs of
            ('*':xs') -> (parseExpr xs') <&> (\(r, ys) -> (Multiply f r, ys))
            _         -> Just (f, xs))

parseExprP1' :: Expr -> String -> Maybe (Expr, String)
parseExprP1' lhs inp = case dropWhile (== ' ') inp of
    ('+':xs) -> rest Add xs
    ('*':xs) -> rest Multiply xs
    _        -> Just (lhs, inp)
    where rest a xs = case parseFactor parseExprP1 xs of
            Just (rhs, ys) -> parseExprP1' (a lhs rhs) ys
            Nothing        -> Nothing

parseExprP1 :: String -> Maybe (Expr, String)
parseExprP1 inp = parseFactor parseExprP1 inp
    >>= uncurry parseExprP1'

eval :: Maybe Expr -> Int
eval e = case e of
    Just (Number x)     -> x
    Just (Add x y)      -> (eval $ Just x) + (eval $ Just y)
    Just (Multiply x y) -> (eval $ Just x) * (eval $ Just y)
    _                   -> 0

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> dupe
    <&> applyT2 (map parseExprP1, map parseExpr)
    <&> applyT2 (map (eval . fmap fst), map (eval . fmap fst))
    <&> applyT2 (sum, sum)
    >>= showParts
