-- prog.hs
-- Copyright (c) 2019, zhiayang
-- Licensed under the Apache License Version 2.0.

{-# LANGUAGE BlockArguments #-}

import Text.Read

readInput :: String -> IO [Int]
readInput filename
    = map read
        . filter (not . null)
        . lines
    <$> readFile filename




calcFuel :: Int -> Int
calcFuel = (flip (-) 2) . (`div` 3)

part1 :: [Int] -> Int
part1 = sum . map calcFuel

part2' :: Int -> Int
part2' = sum . takeWhile (> 0) . (iterate calcFuel)

part2 :: [Int] -> Int
part2 = sum . map (part2' . calcFuel)

main = readInput "input.txt" >>= \inp ->
    putStr $ unlines [  "part 1: " ++ show (part1 inp),
                        "part 2: " ++ show (part2 inp)  ]
