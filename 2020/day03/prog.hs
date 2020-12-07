-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

check' :: Int -> Int -> Int -> [String] -> Int
check' _ _ _ [] = 0
check' x dx dy xs = (if (head xs !! (x `mod` (length $ head xs)) == '#') then
    1 else 0) + check' (x + dx) dx dy (drop dy xs)

check :: Int -> Int -> [String] -> Int
check = check' 0

main :: IO ()
main = readFile "input.txt"
    <&> lines
    <&> dupe
    <&> applyT2 (check 3 1, mapL0 [check 1 1, check 3 1, check 5 1, check 7 1, check 1 2])
    <&> applyT2 (id, product)
    >>= showParts
