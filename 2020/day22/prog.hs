-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Data.Bool
import qualified Data.Set as S

type Deck = [Int]

score :: Deck -> Int
score = sum . zipWith (*) [1..] . reverse

combat :: Deck -> Deck -> Deck
combat a []          = a
combat [] b          = b
combat (a:as) (b:bs) = if a > b
    then combat (as ++ [a, b]) bs
    else combat as (bs ++ [b, a])


recursiveCombat :: S.Set (Deck, Deck) -> Deck -> Deck -> (Deck, Int)
recursiveCombat _ [] bs = (bs, 1)
recursiveCombat _ as [] = (as, 0)
recursiveCombat seen (a:as) (b:bs) = if S.member (a:as, b:bs) seen
    then (a:as, 0)  -- player 1 wins
    else
        let winner = if (length as >= a) && (length bs >= b)
                then snd $ recursiveCombat S.empty (take a as) (take b bs)
                else bool 1 0 (a > b)
        in recursiveCombat seen' (bool as as' (winner == 0)) (bool bs bs' (winner == 1))
    where
        seen' = S.insert (a:as, b:bs) seen
        as' = as ++ [a, b]
        bs' = bs ++ [b, a]

main :: IO ()
main = readFile "input.txt"
    <&> splitByMany "\n\n"
    <&> (\[a, b] -> (map int $ drop 1 $ lines a, map int $ drop 1 $ lines b))
    <&> dupe
    <&> applyT2 (uncurry combat, fst . (uncurry $ recursiveCombat S.empty))
    <&> applyT2 (score, score)
    >>= showParts
