-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

-- format is: 17-34 a: password
parse :: String -> (Int, Int, Char, String)
parse s =
    ( read $ fst $ bisect '-' s
    , read $ snd $ bisect '-' $ fst $ bisect ' ' s
    , head $ snd $ bisect ' ' s
    , snd $ bisect ' ' $ snd $ bisect ' ' s
    )

readInput :: IO [String]
readInput = lines <$> readFile "input.txt"

part1 :: (Int, Int, Char, String) -> Bool
part1 (a, b, c, s) = between a (length $ filter (== c) s) b
    where between a x b = (a <= x && x <= b)

part2 :: (Int, Int, Char, String) -> Bool
part2 (a, b, c, s) = (s !! (a - 1) == c) /= (s !! (b - 1) == c)

main :: IO ()
main = readInput
    <&> map (dupe . parse)
    <&> unzip . mapT1 (part1, part2)
    <&> (applyT1 $ dupe (length . filter id))
    >>= showParts
