-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Data.Functor

check' :: Int -> Int -> Int -> [String] -> Int
check' _ _ _ [] = 0
check' x dx dy xs = (if (head xs !! (x `mod` (length $ head xs)) == '#') then
    1 else 0) + check' (x + dx) dx dy (drop dy xs)

check :: Int -> Int -> [String] -> Int
check = check' 0

readInput :: IO [String]
readInput = lines <$> readFile "input.txt"

main :: IO ()
main = readInput
    <&> (\xs -> (xs, xs))
    <&> (\(a, b) -> (check 3 1 a, [ check 1 1 a, check 3 1 a, check 5 1 a, check 7 1 a, check 1 2 a ]))
    <&> (\(a, bs) -> (a, product bs))
    <&> (\(a, b) -> [ "part 1 = " ++ show a, "part 2 = " ++ show b ])
    <&> unlines
    >>= putStrLn
