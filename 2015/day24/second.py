

from itertools import *
from functools import *

pkgs = {1, 2, 3, 5, 7, 13, 17, 19, 23, 29, 31, 37, 41, 43, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113}
w = sum(pkgs) // 3 # or 4 for part 2

for n1 in range(1, len(pkgs)):
    for g1 in combinations(pkgs, n1):
        if sum(g1) != w: continue
        qe = reduce(lambda a,b: a*b, g1, 1)
        pkgs2 = pkgs - set(g1)
        for n2 in range(1, len(pkgs2)):
            for g2 in combinations(pkgs2, n2):
                if sum(g2) != w: continue
# Part 1...
                # g3 = pkgs2 - set(g2)
                # if sum(g3) != w: continue
                # print(qe)
                # exit(0)
# ...or part 2
                pkgs3 = pkgs2 - set(g2)
                for n3 in range(1, len(pkgs3)):
                    for g3 in combinations(pkgs3, n3):
                        if sum(g3) != w: continue
                        g4 = pkgs3 - set(g3)
                        if sum(g4) != w: continue
                        print(qe)
                        exit(0)
