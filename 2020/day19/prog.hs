-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import qualified Data.Map as M

type Rules = M.Map Int Rule
data Rule = Chr Char | All [Int] | Alt [[Int]] deriving (Show, Eq)

makeRule :: String -> (Int, Rule)
makeRule s = bisect ':' s
    |> (\(n, xs') -> (int n, let xs = trim xs' in if head xs == '"'
        then Chr $ head $ drop 1 xs
        else case elemIndex '|' xs of
            Just _  -> Alt $ map (\subs -> map int $ words $ trim subs) $ splitBy '|' xs
            Nothing -> All $ map int $ words xs
        ))

check :: Rules -> [Rule] -> String -> Bool
check _ [] str               = null str
check _ rs []                = null rs
check rs (rule:rules) (x:xs) = case rule of
    Chr chr  -> if x == chr then (check rs rules xs) else False
    All all  -> check rs (rules `with` all) (x:xs)
    Alt alts -> map (\alt -> check rs (rules `with` alt) (x:xs)) alts
                |> (listToMaybe . filter (id))
                |> maybe False id
    where with :: [Rule] -> [Int] -> [Rule]
          with rules subs = (map ((M.!) rs) subs) ++ rules

modifyInput :: String -> String
modifyInput ('8':':':_)     = "8: 42 | 42 8"
modifyInput ('1':'1':':':_) = "11: 42 31 | 42 11 31"
modifyInput x               = x

main :: IO ()
main = readFile "input.txt"
    <&> splitByMany "\n\n"
    <&> (\[rules, ls] -> ((lines rules, map modifyInput $ lines rules), lines ls))
    <&> (\(rs, ls) -> (applyT2 (dupe $ M.fromList . map makeRule) rs, ls))
    <&> (\((rs1, rs2), ls) -> (map (check rs1 [rs1 M.! 0]) ls, map (check rs2 [rs2 M.! 0]) ls))
    <&> applyT2 (dupe $ countElem True)
    >>= showParts
