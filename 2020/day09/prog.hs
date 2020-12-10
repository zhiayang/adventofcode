-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

pairs :: [Int] -> [(Int, Int)]
pairs xs = [(a, b) | a <- xs, b <- xs]
    |> filter (\(a, b) -> a /= b)

part1 :: [Int] -> Int
part1 xs = windowList 26 xs
    |> map dupe
    |> mapT2 (head . drop 25, take 25)
    |> mapT2 (id, map (uncurry (+)) . pairs)
    |> mapT2 (id, uniqueList)
    |> filter (\(x, xs) -> x `notElem` xs)
    |> head
    |> fst

part2 :: [Int] -> Int
part2 xs = [2 .. (length xs)]
    |> map (flip windowList $ xs)
    |> map (mapT2 (sum, id) . map dupe)
    |> concat
    |> filter ((== target) . fst)
    |> map ((\x -> (head x, last x)) . snd)
    |> head
    |> uncurry (+)
    where target = part1 xs

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> dupe . map int
    <&> applyT2 (part1, part2)
    >>= showParts

