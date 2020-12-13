-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

part1 :: (Int, [Int]) -> Int
part1 (now, times) = map (\n -> iterate (applyT2 ((+ n), id)) (0, n)) times
    |> map (head . dropWhile ((< now) . fst))
    |> (head . sort)
    |> (\(t, b) -> (t - now) * b)

-- returns time
commonTime' :: Int -> Int -> [(Int, Int)] -> Int
commonTime' t adv bs = map (check1 t) bs
    |> (\xs -> and xs)
    |> (\b -> if b then t else commonTime' (t + adv) adv bs)
    where
        check1 t (ofs, bus) = (t + ofs) `mod` bus == 0

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
    |> map (\(nb, bs) -> (take nb bs))
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



