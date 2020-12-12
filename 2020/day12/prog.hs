-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

hdg :: Int -> Int
hdg x
    | x >= 360  = hdg (x - 360)
    | x < 0     = hdg (x + 360)
    | otherwise = x

part1 :: ((Int, Int), Int) -> [(Char, Int)] -> ((Int, Int), Int)
part1 pos [] = pos
part1 ((x, y), h) ((a, n) : xs) = case a of
    'N' -> part1 ((x, y + n), h) xs
    'S' -> part1 ((x, y - n), h) xs
    'E' -> part1 ((x + n, y), h) xs
    'W' -> part1 ((x - n, y), h) xs
    'L' -> part1 ((x, y), hdg $ h - n) xs
    'R' -> part1 ((x, y), hdg $ h + n) xs
    'F' -> part1 ((case h of 0 -> (x, y + n); 90 -> (x + n, y); 180 -> (x, y - n); 270 -> (x - n, y)), h) xs

part2 :: ((Int, Int), (Int, Int)) -> [(Char, Int)] -> ((Int, Int), (Int, Int))
part2 x [] = x
part2 (p@(x, y), w@(wx, wy)) ((a, n) : xs) = case a of
    'N' -> part2 (p, (wx, wy + n)) xs
    'S' -> part2 (p, (wx, wy - n)) xs
    'E' -> part2 (p, (wx + n, wy)) xs
    'W' -> part2 (p, (wx - n, wy)) xs
    'L' -> part2 (p, rot n w) xs
    'R' -> part2 (p, rot (360 - n) w) xs
    'F' -> part2 ((x + n * wx, y + n * wy), w) xs
    where
        rot :: Int -> (Int, Int) -> (Int, Int)
        rot 0 p = p
        rot a p = rot (a - 90) (-1 * snd p, fst p)

dist :: (Int, Int) -> Int
dist (x, y) = abs x + abs y

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> map (\(x:xs) -> (x, int xs))
    <&> dupe
    <&> applyT2 (part1 ((0, 0), 90), part2 ((0, 0), (10, 1)))
    <&> applyT2 (dist . fst, dist . fst)
    >>= showParts
