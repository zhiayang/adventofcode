-- Utils.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

module Utils (module Utils, module Data.Functor) where
import Data.Functor

dupe :: a -> (a, a)
dupe x = (x, x)

-- i know this is like <*>, but that one appears to die if either list is infinite.
mapL :: [a -> b] -> [a] -> [b]
mapL fs = map (\(f, x) -> f x) . zip fs

mapL0 :: [a -> b] -> a -> [b]
mapL0 fs = mapL fs . repeat

mapT1 :: (a -> c, b -> d) -> [(a, b)] -> [(c, d)]
mapT1 (f, g) = map (\(x, y) -> (f x, g y))

applyT1 :: (a -> c, b -> d) -> (a, b) -> (c, d)
applyT1 (f, g) t1 = (f $ fst t1, g $ snd t1)

showParts :: Show a => Show b => (a, b) -> IO ()
showParts (a, b) = putStr $ unlines [ "part 1 = " ++ show a, "part 2 = " ++ show b ]

bisect :: Eq a => a -> [a] -> ([a], [a])
bisect x xs = applyT1 (takeWhile (/= x), dropWhile (== x) . dropWhile (/= x)) $ dupe xs

splitBy :: Eq a => a -> [a] -> [[a]]
splitBy _ [] = []
splitBy x xs = (fst bs) : (splitBy x $ snd bs)
    where bs = bisect x xs
