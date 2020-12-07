-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

listify2 :: (a, a) -> [a]
listify2 (a, b) = [a, b]

listify3 :: (a, a, a) -> [a]
listify3 (a, b, c) = [a, b, c]

part1 :: [Int] -> Int
part1 xs = [(a, b) | a <- xs, b <- xs]
    |> filter ((== 2020) . sum . listify2)
    |> head
    |> listify2
    |> product

part2 :: [Int] -> Int
part2 xs = [(a, b, c) | a <- xs, b <- xs, c <- xs]
    |> filter ((== 2020) . sum . listify3)
    |> head
    |> listify3
    |> product

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> map read
    <&> dupe
    <&> applyT2 (part1, part2)
    >>= showParts
