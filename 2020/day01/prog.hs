-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

listify2 :: (a, a) -> [a]
listify2 (a, b) = [a, b]

listify3 :: (a, a, a) -> [a]
listify3 (a, b, c) = [a, b, c]

part1 :: [Int] -> Int
part1 xs = product . listify2 . head . filter ((== 2020) . sum . listify2) $ [(a, b) | a <- xs, b <- xs]

part2 :: [Int] -> Int
part2 xs = product . listify3 . head . filter ((== 2020) . sum . listify3) $ [(a, b, c) | a <- xs, b <- xs, c <- xs]

readInput :: IO [Int]
readInput = map read . lines <$> readFile "input.txt"

main :: IO ()
main = readInput >>= \inp -> putStr $ unlines
    [ "part 1 = " ++ show (part1 inp),
      "part 2 = " ++ show (part2 inp) ]
