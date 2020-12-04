-- prog.hs
-- Copyright (c) 2020, zhiayang
-- Licensed under the Apache License Version 2.0.

import Utils

data Passport = Passport { byr :: !String, iyr :: !String, eyr :: !String, hgt :: !String
                         , hcl :: !String, ecl :: !String, pid :: !String, cid :: !String }
                         deriving Show

emptyPassport = Passport { byr = "", iyr = "", eyr = "", hgt = "", hcl = "", ecl = "", pid = "", cid = "" }

parsePassport :: [(String, String)] -> Passport
parsePassport [] = emptyPassport
parsePassport vals = case (fst hd) of
    "byr" -> (parsePassport tl) { byr = snd hd }
    "iyr" -> (parsePassport tl) { iyr = snd hd }
    "eyr" -> (parsePassport tl) { eyr = snd hd }
    "hgt" -> (parsePassport tl) { hgt = snd hd }
    "hcl" -> (parsePassport tl) { hcl = snd hd }
    "ecl" -> (parsePassport tl) { ecl = snd hd }
    "pid" -> (parsePassport tl) { pid = snd hd }
    "cid" -> (parsePassport tl) { cid = snd hd }
    where hd = head vals
          tl = tail vals

present :: Passport -> Bool
present (Passport byr iyr eyr hgt hcl ecl pid _) =
    [byr, iyr, eyr, hgt, hcl, ecl, pid]
    |> filter ((== 0) . length)
    |> length
    |> (== 0)

validRange :: Int -> Int -> String -> Bool
validRange min max yr = case readMaybe yr of
    Just x  -> min <= x && x <= max
    Nothing -> False

validHeight :: String -> Bool
validHeight h = if (indexOf "cm" h) /= -1
    then validCm
    else if (indexOf "in" h) /= -1
        then validIn
        else False
    where validCm = validRange 150 193 hn
          validIn = validRange 59 76 hn
          hn = (init $ init h)

validHairColour :: String -> Bool
validHairColour ['#',a,b,c,d,e,f] =
    [a, b, c, d, e, f]
    |> map (flip elem $ "0123456789abcdef")
    |> filter (id . not)
    |> length
    |> (== 0)
validHairColour _ = False

validEyeColour :: String -> Bool
validEyeColour = flip elem $ ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]

validPassportId :: String -> Bool
validPassportId x = (length x == 9) && (isJust $ (readMaybe x :: Maybe Int))

valid :: Passport -> Bool
valid (Passport byr iyr eyr hgt hcl ecl pid _) =
    [ validRange 1920 2002 byr
    , validRange 2010 2020 iyr
    , validRange 2020 2030 eyr
    , validHeight hgt
    , validHairColour hcl
    , validEyeColour ecl
    , validPassportId pid ]
    |> filter (id . not)
    |> length
    |> (== 0)

main :: IO ()
main = readFile "input.txt"
    <&> splitByMany "\n\n"
    <&> map (replace '\n' ' ')
    <&> map (splitBy ' ')
    <&> map (map $ bisect ':')
    <&> map parsePassport
    <&> (uncurry zip) . dupe
    <&> mapT2 (present, valid)
    <&> unzip
    <&> applyT2 (length . filter id, length . filter id)
    >>= showParts
