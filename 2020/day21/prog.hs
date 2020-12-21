-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils
import qualified Data.Set as S
import qualified Data.Map.Strict as M

type Allergen = String
type Ingredient = String
type AllergenMap = M.Map Allergen (S.Set Ingredient)

part1 :: AllergenMap -> [([Allergen], [Ingredient])] -> AllergenMap
part1 am []              = am
part1 am ((as, is) : xs) = part1 am' xs
    where am' = foldl (\m (a, is') -> M.insertWith (S.intersection) a (S.fromList is') m) am $ (zip as $ repeat is)

solve :: [(Allergen, Ingredient)] -> [(Allergen, [Ingredient])] -> [(Allergen, Ingredient)]
solve solns [] = []
solve solns am = am
    |> (filter ((/= 0) . length . snd) . sortBy (comparing (length . snd)))
    |> \xs -> if null xs
        then solns
        else head xs
            |> (\(a, [i]) -> (solns ++ [(a, i)], map (\(a, is) -> (a, delete i is)) am))
            |> (\(s, am) -> solve s am)


part2 :: AllergenMap -> String
part2 am = M.assocs am
    |> map (applyT2 (id, S.elems))
    |> solve []
    |> sortBy (comparing fst)
    |> map snd
    |> intersperse ","
    |> concat

main :: IO ()
main = readFile "input.txt" <&> lines
    <&> map (splitByMany " (contains ")
    <&> map (\[a, b] -> (init b, a))    -- drop the closing ) from the allergens
    <&> mapT2 (splitByMany ", ", words)
    <&> (\xs -> (part1 M.empty xs, concat $ map snd xs))
    <&> (\(am, ais) -> (length $ filter (`notElem` (S.elems $ S.unions $ M.elems am)) ais, part2 am))
    >>= showParts


