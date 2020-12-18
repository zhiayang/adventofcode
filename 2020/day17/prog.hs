-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import qualified Data.Map.Strict as M

type Coord = [Int]
type Grid = M.Map Coord Char

neighbours :: Coord -> Grid -> Int
neighbours pos grid = [
    [dx, dy, dz, dw] | dx <- [-1 .. 1], dy <- [-1 .. 1], dz <- [-1 .. 1], dw <- [-1 .. 1]]
    |> filter (or . map (/= 0))
    |> map (zipWith (+) (pos))
    |> map (\p -> case M.lookup p grid of
        Just c  -> (c == '#')
        Nothing -> False
    )
    |> countElem True

step1 :: Coord -> Coord -> Grid -> Grid
step1 [x1, y1, z1, w1] [x2, y2, z2, w2] grid = [
    [x, y, z, w] | x <- [x1 .. x2], y <- [y1 .. y2], z <- [z1 .. z2], w <- [w1 .. w2]]
    |> map (\p -> let neighs = neighbours p grid in
        case M.findWithDefault '.' p grid of
            '#' -> if neighs `elem` [ 2, 3 ] then (p, '#') else (p, '.')
            '.' -> if neighs `elem` [ 3 ]    then (p, '#') else (p, '.')
    )
    |> M.fromList

step :: Int -> Int -> Coord -> Coord -> Grid -> Grid
step dw 0 _ _ grid   = grid
step dw n p1 p2 grid = step1 p1 p2 grid
    |> step dw (n - 1) p1' p2'
    where p1' = (zipWith (-) p1 [1, 1, 1, dw])
          p2' = (zipWith (+) p2 [1, 1, 1, dw])

main :: IO ()
main = readFile "input.txt"
    <&> zip [0..] . lines
    <&> map (\(y, l) -> map (\(x, c) -> ([x, y, 0, 0], c)) $ zip [0..] l)
    <&> (\l -> (length l, length $ head l, l))
    <&> (\(h, w, l) -> (w, h, M.fromList $ concat l))
    <&> (\(w, h, g) -> (step 0 6 [-1, -1, -1,  0] [w + 1, h + 1, 1, 0] g,
                        step 1 6 [-1, -1, -1, -1] [w + 1, h + 1, 1, 1] g))
    <&> applyT2 (dupe $ M.elems)
    <&> applyT2 (dupe $ countElem '#')
    >>= showParts


