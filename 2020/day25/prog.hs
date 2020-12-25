-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

prime :: Int
prime = 20201227

transform :: Int -> Int -> Int
transform s x = (x * s) `mod` prime

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> map int
    <&> (\[a, b] -> scanl (\(_, i, x, y) k ->
            if (k == a) then      (a, i + 1, i, y)
            else if (k == b) then (a, i + 1, x, i)
            else                  (a, i + 1, x, y)
        ) (a, 0, 0, 0) $ iterate (transform 7) 1)
    <&> (head . dropWhile (\(_, _, x, y) -> (x == 0) || (y == 0)))
    <&> (\(a, _, _, l2) -> (iterate (transform a) 1) !! l2)
    <&> (flip (,) $ "merry fucking christmas")
    >>= showParts


