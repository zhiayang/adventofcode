-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import Data.Bits
import Control.Monad.State
import qualified Data.Map.Strict as M

type Memory = M.Map Int Int
type Operation = Either String (Int, Int)

memsum :: Memory -> Int
memsum = M.foldl (+) 0

mask1 :: String -> Int -> Int
mask1 [] val   = val
mask1 mask val = mask1 (tail mask) (case head mask of
    '0' -> val `clearBit` b
    '1' -> val `setBit`   b
    'X' -> val)
    where b = length mask - 1

part1 :: String -> Memory -> [Operation] -> Memory
part1 mask mem []      = mem
part1 mask mem (op:xs) = case op of
    Left (newMask)    -> part1 newMask mem xs
    Right (addr, val) -> part1 mask (M.insert addr (mask1 mask val) mem) xs

set2 :: String -> Int -> Int -> State Memory ()
set2 [] val addr = do
    mem <- get
    put $ M.insert addr val mem
    return ()
set2 mask val addr = case head mask of
    '0' -> foozle $ addr
    '1' -> foozle $ addr `setBit` b
    'X' -> do
        foozle $ addr `setBit` b
        foozle $ addr `clearBit` b
        return ()
    where
        b = length mask - 1
        foozle = set2 (tail mask) val

part2' :: String -> [Operation] -> State Memory ()
part2' _ []         = return ()
part2' mask (op:xs) = case op of
    Left (newMask)    -> part2' newMask xs
    Right (addr, val) -> do
        set2 mask val addr
        part2' mask xs

part2 :: String -> Memory -> [Operation] -> Memory
part2 mask mem ops = execState (part2' mask ops) mem

parse :: [String] -> [Operation]
parse = map (\line -> case line of
            'm':'a':_ -> Left $ drop 7 line
            'm':'e':_ -> Right $ (int $ drop 4 $ takeWhile (/= ']') line, int $ trim $ snd $ bisect '=' line)
        )

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> parse
    <&> dupe
    <&> applyT2 (part1 "" M.empty, part2 "" M.empty)
    <&> applyT2 (dupe memsum)
    >>= showParts
