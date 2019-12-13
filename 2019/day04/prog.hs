-- prog.hs
-- Copyright (c) 2019, zhiayang
-- Licensed under the Apache License Version 2.0.

{-# LANGUAGE MultiWayIf #-}
{-# LANGUAGE UnicodeSyntax #-}
{-# LANGUAGE BlockArguments #-}
{-# LANGUAGE DerivingStrategies #-}


split :: (Eq a) => a -> [a] -> [[a]]
split _ []      = []
split delim xs  = front : split delim rest
    where (front, rest) = break (== delim) (dropWhile (== delim) xs)

toStr :: Int -> String
toStr = show

checkIncreasing :: String -> Bool
checkIncreasing (a:b:c:d:e:f:_)
    = and [ a <= b, b <= c, c <= d,
            d <= e, e <= f ]
checkIncreasing _ = undefined


checkDouble :: String -> Bool
checkDouble (a:b:c:d:e:f:_)
    = or [ a == b, b == c, c == d, d == e, e == f ]
checkDouble _ = undefined

checkDouble' :: String -> Char -> Bool
checkDouble' (a:b:c:d:e:f:_) x
    = or [  a == x && b == x, b == x && c == x, c == x && d == x,
            d == x && e == x, e == x && f == x ]
checkDouble' _ _ = undefined

checkTriple :: String -> Bool
checkTriple s@(a:b:c:d:e:f:_)
    = any (\x -> not (or [
        x == a && x == b && x == c,
        x == b && x == c && x == d,
        x == c && x == d && x == e,
        x == d && x == e && x == f
    ]) && checkDouble' s x ) ['0'..'9']
checkTriple _ = undefined


-- puzzle input
begin :: Int
begin = 382345

end :: Int
end = 843167

range :: [Int]
range = [ begin .. end ]

part1' :: Int -> Bool
part1' n = (\x -> checkIncreasing x && checkDouble x) (toStr n)

part2' :: Int -> Bool
part2' n = (\x -> checkIncreasing x && checkTriple x) (toStr n)


part1 :: Int
part1 = length $ filter id $ map part1' range

part2 :: Int
part2 = length $ filter id $ map part2' range








