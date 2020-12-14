-- Utils.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

module Utils (
    module Utils,
    -- module Text.Read,
    module Data.Ord,
    module Data.List,
    module Data.Maybe,
    module Data.Functor,
) where

import Text.Read

import Data.Ord
import Data.Char
import Data.List
import Data.Maybe
import Data.Functor

-- OMEGALUL caml
(|>) :: a -> (a -> b) -> b
x |> f = f x

-- duplicates one item into a pair.
dupe :: a -> (a, a)
dupe x = (x, x)

-- applies a list of functions to a list of values, returning the list of results.
-- i know this is like <*>, but that one appears to die if either list is infinite.
mapL :: [a -> b] -> [a] -> [b]
mapL fs = map (\(f, x) -> f x) . zip fs

-- this is like mapL, but instead of using a list of values, applies the various functions
-- to the single value, and collects the list of results.
mapL0 :: [a -> b] -> a -> [b]
mapL0 fs = mapL fs . repeat

mapT2 :: (a -> c, b -> d) -> [(a, b)] -> [(c, d)]
mapT2 (f, g) = map (\(x, y) -> (f x, g y))

applyT2 :: (a -> c, b -> d) -> (a, b) -> (c, d)
applyT2 (f, g) t1 = (f $ fst t1, g $ snd t1)

addT2 :: Num a => Num b => (a, b) -> (a, b) -> (a, b)
addT2 x y = (fst x + fst y, snd x + snd y)


allPairs :: [a] -> [b] -> [(a, b)]
allPairs as bs = [(a, b) | a <- as, b <- bs]

adjPairs :: [a] -> [(a, a)]
adjPairs xs = zip xs (tail xs)

showParts :: Show a => Show b => (a, b) -> IO ()
showParts (a, b) = putStr $ unlines [ "part 1 = " ++ show a, "part 2 = " ++ show b ]

bisect :: Eq a => a -> [a] -> ([a], [a])
bisect x xs = applyT2 (takeWhile (/= x), dropWhile (== x) . dropWhile (/= x)) $ dupe xs

splitBy :: Eq a => a -> [a] -> [[a]]
splitBy _ [] = []
splitBy x xs = (fst bs) : (splitBy x $ snd bs)
    where bs = bisect x xs

indexOf :: Eq a => [a] -> [a] -> Int
indexOf = indexOf' 0 where
    indexOf' i x xs = if (length x) > (length xs)
    then -1
    else if (take (length x) xs) == x
        then i
        else indexOf' (1 + i) x (drop 1 xs)

splitByMany :: Eq a => [a] -> [a] -> [[a]]
splitByMany _ [] = []
splitByMany [] a = [a]
splitByMany x xs = splitByMany' [] x xs where
    splitByMany' ss x xs = if idx /= -1
        then splitByMany' (ss ++ [take idx xs]) x rest
        else if (length xs) == 0 then ss else ss ++ [xs]
        where idx = indexOf x xs
              n = length x
              rest = drop (idx + n) xs

replace :: Eq a => a -> a -> [a] -> [a]
replace _ _ [] = []
replace x y xs = (if h == x then y else h) : (replace x y $ tail xs)
    where h = head xs

trim :: String -> String
trim = (dropWhileEnd isSpace) . (dropWhile isSpace)

int :: String -> Int
int []       = error "empty input"
int ('+':xs) = int xs
int xs       = read xs

uniqueList :: Ord a => [a] -> [a]
uniqueList = map head . group . sort

windowList :: Int -> [a] -> [[a]]
windowList k xs = if (k >= length xs) then [xs]
    else (zip [0..] $ repeat xs)
        |> map (\(n, l) -> take k $ drop n l)
        |> takeWhile ((== k) . length)
