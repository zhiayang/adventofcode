-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

binary :: String -> Int
binary ('1':xs) = (2 ^ (length xs)) + (binary xs)
binary (_:xs) = binary xs
binary [] = 0

triplets :: Eq a => [a] -> [(a, a, a)]
triplets xs = zip3 (xs) (drop 1 xs) (drop 2 xs)

main :: IO ()
main = readFile "input.txt" <&> lines                       -- read input and split into lines
    <&> map (replace 'R' '1' . replace 'B' '1')             -- replace the ints; skip F and L since we just ignore it
    <&> map dupe                                            -- make a pair...
    <&> mapT2 (take 7, drop 7)                              -- then split into the row and column
    <&> mapT2 (dupe binary)                                 -- convert both to binary
    <&> map (\(r, c) -> 8 * r + c)                          -- get the id
    <&> dupe . sort                                         -- sort the list of ids, then dupe for both parts
    <&> applyT2 (last, triplets)                            -- p1: get the max. p2: convert to triplets, to find consec values
    <&> applyT2 (id, filter (\(a, b, c) -> (b + 2 == c)))   -- p2: find the triple with a missing value
    <&> applyT2 (id, head)                                  -- p2: get the first one (should be only one)
    <&> applyT2 (id, \(_, x, _) -> x + 1)                   -- p2: get the missing value itself
    >>= showParts
