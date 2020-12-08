-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

type Instr = (String, Int)

run :: [Instr] -> Int -> (Int, Int, [Int]) -> (Int, Int, [Int])
run xs swap (ip, acc, seen) = if (ip < length xs)
    then case (xs !! ip) of
        ("nop", n) -> (if swap /= ip then nop else jmp) n
        ("jmp", n) -> (if swap /= ip then jmp else nop) n
        ("acc", n) -> (ip + 1, acc + n, seen ++ [ip])
    else
        (-1, acc, seen)
    where
        nop n = (ip + 1, acc, seen ++ [ip])
        jmp n = (ip + n, acc, seen ++ [ip])

part1 :: [Instr] -> Int
part1 ins = iterate (run ins (-1)) (0, 0, [])
    |> dropWhile (\(ip, acc, seen) -> ip `notElem` seen)
    |> head
    |> \(_, acc, _) -> acc

part2 :: [Instr] -> Int
part2 ins = zip [0 .. (length ins - 1)] (repeat ins)
    |> map (\(k, ins) -> iterate (run ins k) (0, 0, []))
    |> map (dropWhile (\(ip, acc, seen) -> (ip `notElem` seen && ip /= -1)))
    |> map head
    |> filter (\(ip, _, _) -> ip == -1)
    |> head
    |> \(_, acc, _) -> acc

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> map (bisect ' ')
    <&> mapT2 (id, int)
    <&> dupe
    <&> applyT2 (part1, part2)
    >>= showParts
