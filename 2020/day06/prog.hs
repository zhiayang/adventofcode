-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Data.Char

-- takes a list of indices, and returns a new list where there is a '1' in an index `i`
-- if and only if `i` appeared in the input list. the input list must be sorted. the first
-- arugment is length of the output list.
indexify :: Int -> [Int] -> [Int]
indexify 0 _ = []
indexify l [] = take l $ repeat 0
indexify l (x:xs) = [ if (x == 0) then 1 else 0 ] ++ (indexify (l - 1) (map (flip (-) $ 1) xss))
    where xss = if (x == 0) then xs else x:xs

main :: IO ()
main = readFile "input.txt"
    <&> splitByMany "\n\n"                                      -- split into groups
    <&> map (map sort . lines)                                  -- split each group into individuals
    <&> mapT2 (id, length) . (map dupe)                         -- also track the size of the group for later
    <&> mapT2 (map (map ord), id)                               -- get the ascii values
    <&> mapT2 (map (map (flip (-) 97)), id)                     -- convert to relative ascii (a=0, b=1, ...)
    <&> mapT2 (map $ indexify 26, id)                           -- convert to "one hot encoding"
    <&> mapT2 (foldl (zipWith (+)) (take 26 $ repeat 0), id)    -- sum the results for each group
    <&> map (\(xs, l) -> (xs, (xs, l)))                         -- restructure the tuple slightly
    <&> mapT2 (filter (> 0), \(xs, l) -> filter (== l) xs)      -- p1: any answered yes; p2: all answered yes
    <&> unzip . mapT2 (length, length)                          -- get the counts
    <&> applyT2 (sum, sum)                                      -- sum
    >>= showParts

