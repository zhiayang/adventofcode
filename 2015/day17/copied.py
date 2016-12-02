from collections import defaultdict
dimensions = [33, 14, 18, 20, 45, 35, 16, 35, 1, 13, 18, 13, 50, 44, 48, 6, 24, 41, 30, 42]

dist = defaultdict(int)
for mask in xrange(1, 1<<len(dimensions)):
    p = [d for i,d in enumerate(dimensions) if (mask & (1 << i)) > 0]
    if sum(p) == 150: dist[len(p)] += 1

print "total:", sum(dist.values())
print "min:", dist[min(dist.keys())]
