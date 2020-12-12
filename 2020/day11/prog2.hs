-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Data.Array

type Grid = ((Int, Int), Array Int Char)

height :: Grid -> Int
height ((_, h), _) = h

width :: Grid -> Int
width ((w, _), _) = w

occupied :: Grid -> Int
occupied g = snd g
    |> elems
    |> filter (== '#')
    |> length

grid :: Grid -> Int -> Int -> Char
grid g x y = if 0 < i && i < s then (snd g) ! (i) else '@'
    where
        (_, s) = bounds $ snd g
        i = y * w + x
        w = width g


dirs :: [(Int, Int)]
dirs = [ (0, 1), (0, -1), (1, 0), (-1, 0), (1, 1), (1, -1), (-1, 1), (-1, -1) ]

getAdj1 :: Grid -> (Int, Int) -> Int
getAdj1 g pos = zipWith addT2 (repeat pos) dirs
    |> map (uncurry $ grid g)
    |> filter (== '#')
    |> length

-- getAdj2 :: Grid -> (Int, Int) -> Int
-- getAdj2 g pos = iterate (\x -> zipWith addT2 dirs x) (repeat pos)
--     |> (transpose . tail)
--     |> take (length dirs)
--     |> map (map $ uncurry $ grid g)
--     |> map (head . dropWhile (== '.'))
--     |> filter (== '#')
--     |> length

sim :: (Grid -> (Int, Int) -> Int) -> Int -> Grid -> Grid
sim adj lim g@((h, w), gg) = allPairs [0 .. h - 1] [0 .. w - 1]
    |> dupe
    |> applyT2 (map $ adj g, map $ uncurry $ grid g)
    |> uncurry zip
    |> map (\(a, p) ->
        if (p == '#' && a >= lim) then      'L'
        else if (p == 'L' && a == 0) then   '#'
        else                                p)
    |> (\xs -> ((h, w), listArray (bounds gg) xs))


    -- |> dupe
    -- |> applyT2 (adj g, uncurry (grid g))
    -- |> uncurry zip

part1 :: Grid -> Int
part1 g = iterate (sim getAdj1 4) g
    |> adjPairs
    |> dropWhile (uncurry (/=))
    |> (fst . head)
    |> occupied

-- part2 :: Grid -> Int
-- part2 g = iterate (sim getAdj2 5) g
--     |> adjPairs
--     |> dropWhile (uncurry (/=))
--     |> (fst . head)
--     |> occupied

main :: IO ()
main = readFile "sample.txt"
    <&> (\a -> (a, listArray (0, length a - 1) a))
    <&> (\(l, a) -> ((length $ head $ lines l, length $ lines l), a))
    <&> part1
    <&> show
    >>= putStrLn

-- main :: IO ()
-- main = readFile "input.txt" <&> lines
--     <&> dupe
--     <&> applyT2 (part1, part2)
--     >>= showParts
