-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

type Graph = [(String, [(Int, String)])]

search2 :: String -> Graph -> Int
search2 bag g = case lookup bag g of
    Just xs -> sum $ map (\(n, x) -> n * (1 + search2 x g)) xs
    Nothing -> 0

search1 :: String -> Graph -> Int
search1 s = length . map head . group . sort . search1' s
    where
        search1' bag g = case lookup bag g of
            Just xs -> concat $ map (\(_, b) -> b : search1' b g) xs
            Nothing -> []

invertGraph' :: Graph -> [(String, [String])]
invertGraph' g = g
    |> map (\(x, xs) -> map (\(_, y) -> (y, x)) xs)
    |> concat
    |> sortBy (comparing fst)
    |> groupBy (\a b -> fst a == fst b)
    |> map (applyT2 (head, id) . unzip)

invertGraph :: Graph -> Graph
invertGraph = map (applyT2 (id, map ((,) 1))) . invertGraph'

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> map (splitByMany "bags contain" . init)
    <&> map (\a -> (trim $ head a, trim $ head $ tail a))
    <&> filter ((/= "no other bags") . snd)
    <&> mapT2 (trim, splitByMany "," . trim)
    <&> mapT2 (id, map (trim . \s -> take (indexOf "bag" s) s))
    <&> mapT2 (id, map $ bisect ' ')
    <&> mapT2 (id, map $ applyT2 (int, id))
    <&> applyT2 (invertGraph, id) . dupe
    <&> applyT2 (search1 "shiny gold", search2 "shiny gold")
    >>= showParts
