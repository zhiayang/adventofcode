-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Data.Bool

part1 :: (Int, [Int]) -> Int
part1 (now, times) = map (\n -> ([0, n ..], n)) times   -- generate [0, n, 2n, 3n, ...] for each n
    |> map (\(l, n) -> (head $ dropWhile (< now) l, n)) -- drop those that are before 'now'
    |> (head . sortBy (comparing fst))                  -- get the earliest one
    |> (\(t, b) -> (t - now) * b)                       -- calculate the answer

-- returns time
commonTime' :: Int -> Int -> [(Int, Int)] -> Int
commonTime' t adv bs = map (\(ofs, bus) -> (t + ofs) `mod` bus == 0) bs
    |> foldl (&&) True
    |> bool (commonTime' (t + adv) adv bs) (t)

-- returns (time, period)
commonTime :: Int -> Int -> [(Int, Int)] -> (Int, Int)
commonTime t adv bs = (t1, t2 - t1)
    where t1 = commonTime' t adv bs
          t2 = commonTime' (t1 + adv) adv bs

part2 :: [String] -> Int
part2 times = zip [0..] times
    |> filter (\(a, b) -> b /= "x")
    |> mapT2 (id, int)
    |> sortBy (comparing snd)
    |> (zip [2 .. num_busses] . repeat)
    |> map (uncurry take)
    |> foldl (\(t, a) bs -> commonTime t a bs) (0, 1)
    |> fst
    where num_busses = length $ filter (/= "x") times

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> (\(x:xs) -> (int x, splitBy ',' $ head xs))
    <&> dupe
    <&> applyT2 (\(n, bs) -> (n, map int $ filter (/= "x") bs), snd)
    <&> applyT2 (part1, part2)
    >>= showParts



