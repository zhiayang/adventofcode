#!/usr/bin/env python

from random import shuffle

transforms = []
molecule = ''
with open('input') as fd:
    lines = [line.strip() for line in fd]
    for line in lines:
        if '=>' in line:
            frm, _, to = line.split()
            transforms.append((frm, to))
        else:
            molecule = line

count = shuffles = 0
mol = molecule
while len(mol) > 1:
    start = mol
    for frm, to in transforms:
        while to in mol:
            count += mol.count(to)
            mol = mol.replace(to, frm)

    if start == mol:  # no progress start again
        shuffle(transforms)
        mol = molecule
        count = 0
        shuffles += 1

print('{} transforms after {} shuffles'.format(count, shuffles))
