-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import qualified Data.Map.Strict as M

type Nums = M.Map Int (Int, Int)

turn :: Int -> Int -> Int -> Nums -> (Nums, Int)
turn stop t last nums = if t == (stop + 1) then (nums, last)
    else turn (stop) (t + 1) (next) $! M.alter (\existing -> case existing of
        Just (a, b) -> Just $ (b, t)
        Nothing     -> Just $ (0, t)
    ) next nums
    where next = case M.lookup last nums of
            Just (a, b) -> if (a == 0 || b == 0) then 0 else b - a
            Nothing     -> 0

solve :: Int -> [Int] -> Int
solve num xs = zip [1..] xs
    |> map (\(i, x) -> (x, (0, i)))
    |> (\l -> (M.fromList l, fst $ last l))
    |> (\(m, l) -> turn num (1 + length xs) l m)
    |> snd

main :: IO ()
main = readFile "input.txt"
    <&> splitBy ','
    <&> map int
    <&> dupe
    <&> applyT2 (solve 2020, solve 30000000)
    >>= showParts




