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
        min1 = int $ fst $ bisect '-' $ head $ nums,
        max1 = int $ snd $ bisect '-' $ head $ nums,
        min2 = int $ fst $ bisect '-' $ last $ nums,
        max2 = int $ snd $ bisect '-' $ last $ nums
    } where nums = splitByMany " or " $ trim $ snd $ bisect ':' c

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
findMapping cons tickets invs mapping = if length mapping == length cons
    then mapping                                                    -- if we found everything, return.
    else cons
        |> filter (isNothing . (flip lookup $ mapping))             -- skip those that we already found
        |> map (\c -> (filter (`notElem` invs) tickets              -- for each constraint (c): 1. skip invalid tickets
            |> transpose                                            -- 2. transpose to get [field=[ticket]] instead of [ticket=[field]]
            |> map (\fs -> foldl (\r f -> r && check c f) True fs)  -- 3. check (c) for each field of all tickets (this is why transpose)
            |> (\fs -> map (\(i, f) ->                              -- 4. for each field (or rather its index):
                if isJust $ find (\(_, i') -> i' == i) mapping      --    - if there is a constraint mapped to it already,
                    then False                                      --    > return False so we don't try to use it again
                    else f                                          --    > else, just return the field's original True/False
               ) $ zip [0..] fs)                                    -- <- this generates the field indices
            |> (\fs -> (c, fs, countElem True fs))                  -- 5. count the number of Trues for each field.
        ))
        |> (head . filter (\(_, _, i) -> i == 1))                   -- now, filter constrs that have only 1 True value
        |> (\(c, fs, _) -> mapping ++ [(c, indexOf [True] fs)])     -- and add it to the solve dmapping
        |> findMapping cons tickets invs                            -- recurse to find the rest

part2 :: Ticket -> [(Constraint, Int)] -> Int
part2 self constrs = constrs
    |> filter ((== 0) . indexOf "departure" . name . fst)
    |> map (\(_, i) -> self !! i)
    |> product

main :: IO ()
main = readFile "input.txt"
    <&> splitByMany "\n\n"                                                      -- split into the 3 parts
    <&> (\[cons, self, tickets] ->                                              -- which are the constraints, self, and others
            ( map makeCons $ lines cons
            , makeTicket $ last $ lines self
            , map makeTicket $ drop 1 $ lines tickets))
    <&> (\(c, s, t) ->                                                          -- solve part1 here, which is quite simple. we
            (let invs = part1 c t in (sum $ map snd $ invs,                     -- also need the list of invalid tickets from
             invs, c, s, t)))                                                   -- part1 for solving part2
    <&> (\(p1, invs, c, s, t) -> (p1, s, findMapping c t (map fst invs) []))    -- find the mapping of constraint to field index
    <&> (\(p1, s, cs) -> (p1, part2 s cs))                                      -- solve part2
    >>= showParts
