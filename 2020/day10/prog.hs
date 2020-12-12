-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import qualified Data.Set as S
import qualified Data.Map.Strict as M

part1 :: [Int] -> Int
part1 xs = pairs xs
    |> map (uncurry $ flip (-))
    |> dupe
    |> applyT2 (length . filter (== 1), length . filter (== 3))
    |> uncurry (*)
    where pairs xs = zip xs (tail xs)

type Memoise = M.Map Int Int

part2 :: [Int] -> Int
part2 xs = fst $ part2' 0 M.empty
    where
        nums = S.fromDistinctAscList xs
        part2' :: Int -> Memoise -> (Int, Memoise)
        part2' x seen =
            if (x == S.findMax nums) then (1, seen)
            else case (S.member x nums, M.lookup x seen) of
                (False, _)      -> (0, seen)
                (True, Just xx) -> (xx, seen)
                (True, Nothing) -> (ans, M.insert x ans s)
                    where collapse (a, b) f = f b |> applyT2 ((+) a, id)
                          (ans, s) = foldl collapse (0, seen) $ map (part2' . (+) x) [1..3]

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> sort . map int
    <&> (\xs -> [ 0 ] ++ xs ++ [last xs + 3])
    <&> dupe
    <&> applyT2 (part1, part2)
    >>= showParts

