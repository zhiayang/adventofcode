-- prog.hs
-- Copyright (c) 2019, zhiayang
-- Licensed under the Apache License Version 2.0.

{-# LANGUAGE MultiWayIf #-}
{-# LANGUAGE UnicodeSyntax #-}
{-# LANGUAGE BlockArguments #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE DerivingStrategies #-}

import Data.List
import qualified Data.Map.Strict as Map

split :: (Eq a) => a -> [a] -> [[a]]
split _ []      = []
split delim xs  = front : split delim rest
    where (front, rest) = break (== delim) (dropWhile (== delim) xs)

newtype V2 = V2 { xy :: (Int, Int) }
    deriving (Eq, Ord, Show)

type Map = Map.Map V2 Int

instance Num V2 where
    (+) (V2(a, b)) (V2(c, d))   = V2(a+c, b+d)
    abs (V2(x, y))              = V2(abs x, abs y)
    negate (V2(x, y))           = V2(negate x, negate y)
    (*)                         = undefined
    signum                      = undefined
    fromInteger x               = V2(fromInteger x, fromInteger x)

magnitude :: V2 -> Int
magnitude v
    = abs (fst (xy v)) + abs (snd (xy v))

readLines :: String -> IO [String]
readLines filename
    = filter (not . null) . lines <$> readFile filename

getMoves :: String -> [String]
getMoves line
    = filter (not . null) (split ',' line)

getDir :: Char -> V2
getDir 'U' = V2(0, -1)
getDir 'D' = V2(0,  1)
getDir 'L' = V2(-1, 0)
getDir 'R' = V2( 1, 0)
getDir _   = undefined


-- params: (U|D|L|R), count, cur_pos, stepcount, map
-- return: new map
move' :: Char -> Int -> V2 -> Int -> Map -> (Map, (V2, Int))
move' _ 0 p s m = (m, (p, s))
move' d count pos steps m
    = move' d (count - 1) newpos (steps + 1) $ Map.insert newpos (steps + 1) m
    where newpos = getDir d + pos

-- params: movestring (eg. U1234), cur_pos, stepcount, map
-- return: new map
move :: String -> V2 -> Int -> Map -> (Map, (V2, Int))
move (x:n)  = move' x $ read n
move _      = undefined

zeroV2 :: V2
zeroV2 = V2(0, 0)


getPath :: [String] -> Map
getPath moves
    = fst $ foldl (\(st, (pos, steps)) mv -> move mv pos steps st) (start, (zeroV2, 0)) moves
    where start = Map.singleton zeroV2 0


part1 :: [String] -> [String] -> V2
part1 mv1 mv2
    = head $ sortOn magnitude xs
    where xs = filter (/= zeroV2) $ map fst $ Map.toList $ Map.intersection (getPath mv1) (getPath mv2)


part2 :: [String] -> [String] -> Int
part2 mv1 mv2
    = minimum $ map (\x -> m1 Map.! x + m2 Map.! x) xs
    where
        m1 = getPath mv1
        m2 = getPath mv2
        xs = filter (/= zeroV2) $ map fst $ Map.toList $ Map.intersection m1 m2

main :: IO ()
main = do
    (mv1:mv2:_) <- readLines "input.txt"
    let m1 = getMoves mv1
        m2 = getMoves mv2
    putStrLn ("part 1: " ++ show (magnitude (part1 m1 m2)))
    putStrLn ("part 2: " ++ show (part2 m1 m2))









