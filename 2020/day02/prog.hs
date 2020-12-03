-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Data.Functor

parse :: String -> (Int, Int, Char, String)
parse s =
    ( read $ takeWhile (/= '-') $ takeWhile (/= ' ') s
    , read $ tail $ dropWhile (/= '-') $ takeWhile (/= ' ') s
    , head $ tail $ dropWhile (/= ' ') s
    , tail $ dropWhile (/= ' ') $ tail $ dropWhile (/= ' ') s
    )

readInput :: IO [String]
readInput = lines <$> readFile "input.txt"

part1 :: (Int, Int, Char, String) -> Bool
part1 (a, b, c, s) = between a (length $ filter (== c) s) b
    where between a x b = (a <= x && x <= b)

part2 :: (Int, Int, Char, String) -> Bool
part2 (a, b, c, s) = (s !! (a - 1) == c) /= (s !! (b - 1) == c)

main :: IO ()
main = readInput
    <&> map parse
    <&> unzip . map (\xs -> (part1 xs, part2 xs))
    <&> (\(a, b) -> (length $ filter id a, length $ filter id b))
    <&> (\(a, b) -> [ "part 1 = " ++ show a, "part 2 = " ++ show b ])
    <&> unlines
    >>= putStr
