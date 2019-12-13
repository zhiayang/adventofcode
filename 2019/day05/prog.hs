-- prog.hs
-- Copyright (c) 2019, zhiayang
-- Licensed under the Apache License Version 2.0.

{-# LANGUAGE MultiWayIf #-}
{-# LANGUAGE UnicodeSyntax #-}
{-# LANGUAGE BlockArguments #-}
{-# LANGUAGE OverloadedStrings #-}

import Data.Int
import Data.Maybe
import qualified Data.Map.Strict as Map

infixr 9 ∘
(∘) :: (b -> c) -> (a -> b) -> a -> c
(∘) f g = f . g

cartesian :: [a] -> [b] -> [(a, b)]
cartesian xs ys
    = [(x,y) | x <- xs, y <- ys]


data IntCodeVM = IntCodeVM {
    ip      :: Int64,
    segment :: Int64,
    memory  :: Map.Map Int64 Int64,
    input   :: [Int64],
    output  :: [Int64],
    stopped :: Bool
} deriving Show



split :: (Eq a) => a -> [a] -> [[a]]
split _ []      = []
split delim xs  = front : split delim rest
    where (front, rest) = break (== delim) (dropWhile (== delim) xs)


readInput :: String -> IO [Int]
readInput filename
    = map read
        ∘ filter (not ∘ null)
        ∘ split ','
    <$> readFile filename



-- we'll just do a full intcode interpreter here.
setupMemory :: [Int] -> Map.Map Int64 Int64
setupMemory = Map.fromAscList ∘ zip [0 .. ] ∘ map fromIntegral

makeVM :: [Int] -> IntCodeVM
makeVM mem = IntCodeVM {
    ip = 0, segment = 0,
    input = [], output = [],
    memory = setupMemory mem,
    stopped = False
}

getInstrLen :: Int64 -> Int64
getInstrLen op
    | op `elem` [1, 2, 7, 8] = 4
    | op `elem` [5, 6]       = 3
    | op `elem` [3, 4, 9]    = 2
    | otherwise              = 1

-- params: operand num, opcode
-- return: mode (0, 1, or 2)
getOperandMode :: Int64 -> Int64 -> Int64
getOperandMode 0 = (`div` 100)   ∘ (`rem` 1000)
getOperandMode 1 = (`div` 1000)  ∘ (`rem` 10000)
getOperandMode 2 = (`div` 10000) ∘ (`rem` 100000)
getOperandMode _ = undefined


-- params: addr, vm
-- return: value
getMem :: Int64 -> IntCodeVM -> Int64
getMem addr
    -- returns 0 for uninitialised memory.
    = fromMaybe 0 ∘ Map.lookup addr ∘ memory


-- params: addr, value, vm
-- return: new vm
setMem :: Int64 -> Int64 -> IntCodeVM -> IntCodeVM
setMem addr value vm
    = vm { memory = Map.insert addr value mem }
    where mem = memory vm


-- params: opcode, operand num, vm
-- return: value
getOperand :: Int64 -> Int64 -> IntCodeVM -> Int64
getOperand op i vm
    | mode == 0     = getMem (getMem (ip vm + 1 + i) vm) vm
    | mode == 1     = getMem (ip vm + 1 + i) vm
    | mode == 2     = getMem (segment vm + getMem (ip vm + 1 + i) vm) vm
    | otherwise     = undefined
    where
        mode = getOperandMode i op


-- params: vm, opcode, argnum, value
-- return: new vm
setResult :: IntCodeVM -> Int64 -> Int64 -> Int64 -> IntCodeVM
setResult vm op i value
    = case mode of
        0 -> setMem addr value vm
        2 -> setMem (addr + segment vm) value vm
        _ -> undefined
    where
        mode = getOperandMode i op
        addr = getMem (ip vm + 1 + i) vm


-- params: opcode, vm
-- return: new vm
incrIp :: Int64 -> IntCodeVM -> IntCodeVM
incrIp op vm
    = vm { ip = ip vm + getInstrLen op }

-- params: vm, opcode
-- return: new vm
runInstr :: Int64 -> IntCodeVM -> IntCodeVM
runInstr op' vm
    = let op = op' `rem` 100 in
        case op of
        1  -> incrIp op $ setResult vm op 2 (getOp 0 vm + getOp 1 vm)
        2  -> incrIp op $ setResult vm op 2 (getOp 0 vm * getOp 1 vm)
        3  -> incrIp op $ (setResult vm op 0 (head $ input vm)) { input = init (input vm) }
        4  -> incrIp op $ vm { output = output vm ++ [getOp 0 vm] }
        5  -> let cond = getOp 0 vm in if
            | cond /= 0 -> vm { ip = getOp 1 vm }
            | otherwise -> incrIp op vm
        6  -> let cond = getOp 0 vm in if
            | cond == 0 -> vm { ip = getOp 1 vm }
            | otherwise -> incrIp op vm
        7  -> incrIp op
            $ let a = getOp 0 vm
                  b = getOp 1 vm in
             setResult vm op 2 if
                | a < b     -> 1
                | otherwise -> 0
        8  -> incrIp op
            $ let a = getOp 0 vm
                  b = getOp 1 vm in
            setResult vm op 2 if
                | a == b    -> 1
                | otherwise -> 0
        9  -> undefined
        99 -> vm { stopped = True }
        _  -> undefined
    where
        getOp = getOperand op'


-- params: vm
-- return: new vm
run :: IntCodeVM -> IntCodeVM
run vm
    | stopped vm    = vm
    | otherwise     = (run ∘ runInstr (getMem (ip vm) vm)) vm

-- params: vm, input
-- return: mem[0]
runVM :: IntCodeVM -> Int64 -> Int64
runVM vm inp
    = last
    $ output
    $ run
    $ vm { input = [ inp ] }

main :: IO ()
main = do
    mem <- readInput "input.txt"
    let vm  = makeVM mem in
        putStr $ unlines [
            "part 1: " ++ show (runVM vm 1),
            "part 2: " ++ show (runVM vm 5)
        ]








