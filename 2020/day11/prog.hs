-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

type Grid = [[Char]]

height :: Grid -> Int
height g = length g

width :: Grid -> Int
width g = length $ head g

occupied :: Grid -> Int
occupied = sum . map (length . filter (== '#'))

grid :: Grid -> Int -> Int -> Char
grid g y x = if (x < 0 || y < 0 || x >= width g || y >= height g)
    then '@' else (g !! y) !! x

gridOf :: Int -> [Char] -> Grid
gridOf _ [] = []
gridOf w xs = [take w xs] ++ gridOf w (drop w xs)

dirs :: [(Int, Int)]
dirs = [ (0, 1), (0, -1), (1, 0), (-1, 0), (1, 1), (1, -1), (-1, 1), (-1, -1) ]

getAdj1 :: Grid -> (Int, Int) -> Int
getAdj1 g pos = zipWith addT2 (repeat pos) dirs
    |> map (uncurry $ grid g)
    |> filter (== '#')
    |> length

getAdj2 :: Grid -> (Int, Int) -> Int
getAdj2 g pos = iterate (\x -> zipWith addT2 dirs x) (repeat pos)
    |> (transpose . tail)
    |> take (length dirs)
    |> map (map $ uncurry $ grid g)
    |> map (head . dropWhile (== '.'))
    |> filter (== '#')
    |> length

sim :: (Grid -> (Int, Int) -> Int) -> Int -> Grid -> Grid
sim adj lim g = allPairs [0 .. height g - 1] [0 .. width g - 1]
    |> dupe
    |> applyT2 (map $ adj g, map $ uncurry (grid g))
    |> uncurry zip
    |> map (\(a, p) ->
        if (p == '#' && a >= lim) then      'L'
        else if (p == 'L' && a == 0) then   '#'
        else                                p)
    |> gridOf (width g)

part1 :: Grid -> Int
part1 g = iterate (sim getAdj1 4) g
    |> adjPairs
    |> dropWhile (uncurry (/=))
    |> (fst . head)
    |> occupied

part2 :: Grid -> Int
part2 g = iterate (sim getAdj2 5) g
    |> adjPairs
    |> dropWhile (uncurry (/=))
    |> (fst . head)
    |> occupied

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> dupe
    <&> applyT2 (part1, part2)
    >>= showParts
