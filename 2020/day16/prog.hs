-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Debug.Trace

type Ticket = [Int]
data Constraint = Constraint { name :: String, min1 :: Int, max1 :: Int
                             , min2 :: Int, max2 :: Int
                             } deriving(Show, Eq)

makeCons :: String -> Constraint
makeCons c = Constraint {
    name = fst $ bisect ':' c,
    min1 = int $ fst $ bisect '-' $ head $ splitByMany " or " $ trim $ snd $ bisect ':' c,
    max1 = int $ snd $ bisect '-' $ head $ splitByMany " or " $ trim $ snd $ bisect ':' c,
    min2 = int $ fst $ bisect '-' $ last $ splitByMany " or " $ trim $ snd $ bisect ':' c,
    max2 = int $ snd $ bisect '-' $ last $ splitByMany " or " $ trim $ snd $ bisect ':' c
}

check :: Constraint -> Int -> Bool
check c i = (min1 c <= i && i <= max1 c) || (min2 c <= i && i <= max2 c)

makeTicket :: String -> Ticket
makeTicket = map int . splitBy ','

part1 :: [Constraint] -> [Ticket] -> [(Ticket, Int)]
part1 cons tickets = tickets
    |> map (\t -> (t, map (\f -> (f, foldl (\r c -> r && (not $ check c f)) True cons)) t))
    |> map (\(t, fs) -> (t, map (\(f, bs) -> if bs then f else 0) fs))
    |> map (\(t, l) -> (t, sum l))
    |> filter (\(t, l) -> l /= 0)

findMapping :: [Constraint] -> [Ticket] -> [Ticket] -> [(Constraint, Int)] -> [(Constraint, Int)]
findMapping cons tickets invs mapping = if length mapping == length cons then mapping
    else cons
    |> filter (isNothing . (flip lookup $ mapping))
    |> map (\c -> (filter (`notElem` invs) tickets
        |> transpose
        |> map (\fs -> foldl (\r f -> r && check c f) True fs)
        |> (\fs -> map (\(i, f) -> if isJust $ find (\(_, i') -> i' == i) mapping
            then False
            else f
           ) $ zip [0..] fs)
        |> (\fs -> (c, fs, countElem True fs))
    ))
    |> (head . filter (\(_, _, i) -> i == 1))
    |> (\(c, fs, _) -> mapping ++ [(c, indexOf [True] fs)])
    |> findMapping cons tickets invs

part2 :: Ticket -> [(Constraint, Int)] -> Int
part2 self constrs = constrs
    |> filter ((== 0) . indexOf "departure" . name . fst)
    |> map (\(_, i) -> self !! i)
    |> product

main :: IO ()
main = readFile "input.txt"
    <&> splitByMany "\n\n"
    <&> (\[a, b, c] -> (a, b, c))
    <&> (\(cons, self, tickets) -> (map makeCons $ lines cons
        , makeTicket $ last $ lines self
        , map makeTicket $ drop 1 $ lines tickets))
    <&> (\(c, s, t) -> (let invs = part1 c t in (sum $ map snd $ invs, invs, c, s, t)))
    <&> (\(p1, invs, c, s, t) -> (p1, s, findMapping c t (map fst invs) []))
    <&> (\(p1, s, cs) -> (p1, part2 s cs))
    >>= showParts
