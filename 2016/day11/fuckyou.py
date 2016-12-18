from collections import deque, namedtuple
import sys
import time

indicies = ['pog', 'pom', 'tmg', 'tmm', 'pmg', 'pmm',
            'rug', 'rum', 'cog', 'com', 'elg', 'elm', 'dlg', 'dlm']

# idx = generator loc
# idx + 1 = chip loc
part1 = [1, 2, 1, 1, 1, 2, 1, 1, 1, 1]
part2 = [1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1]


State = namedtuple('State', ['f', 'e', 's', 'p'])


def valid(state):
    if not 1 <= state.e <= 4:
        return False
    if any(not 1 <= i <= 4 for i in state.f):
        return False

    for idx, v in enumerate(state.f[1::2]):
        idx = idx * 2 + 1
        if v != state.f[idx - 1] and any(v == i for i in state.f[0::2]):
            return False
    return True


def generalize(state):
    g = [sum(1 for v in state.f[::2] if v == fn) for fn in range(1, 5)]
    m = [sum(1 for v in state.f[1::2] if v == fn) for fn in range(1, 5)]
    return ''.join(map(str, g + m)) + str(state.e)


# def less_generalized(state):
#     return ''.join(map(str, (state.f, state.e)))


def solved(state):
    return all(i == 4 for i in state.f)


def make_animation(state):
    parents = []
    while state:
        parents.append(state)
        state = state.p
    parents.reverse()
    for p in parents:
        sys.stdout.write('\033c')
        for fn in range(1, 5):
            out = 'F%s: ' % fn
            out += ' '.join(indicies[idx].upper()
                            if i == fn else '   ' for idx, i in enumerate(p.f))
            if fn == p.e:
                out += ' [E]'
            print out
        time.sleep(0.2)

    print('\n')


def bfs(floor_config, part, animate=True):
    bfs_q = deque()
    bfs_q.append(State(floor_config, 1, 0, None))
    seen = set()
    nodes = 1
    while bfs_q:
        state = bfs_q.popleft()
        if generalize(state) in seen or not valid(state):
            continue
        seen.add(generalize(state))

        if solved(state):
            if animate:
                make_animation(state)
            print "Part %s:\tSolved in %s steps." % (part, state.s)
            print "\tConsidered %s nodes." % nodes
            return

        for idx in range(len(state.f)):
            i = state.f[idx]
            if i != state.e:  # item can't be moved bc not in elevator
                continue
            state.f[idx] -= 1
            bfs_q.append(State(list(state.f), state.e - 1, state.s + 1, state))
            state.f[idx] += 2
            bfs_q.append(State(list(state.f), state.e + 1, state.s + 1, state))
            state.f[idx] -= 1

            nodes += 2

            for jdx in range(idx + 1, len(state.f)):
                if state.f[jdx] != state.e:
                    continue
                state.f[jdx] -= 1
                state.f[idx] -= 1
                bfs_q.append(State(list(state.f), state.e - 1,
                                   state.s + 1, state))
                state.f[jdx] += 2
                state.f[idx] += 2
                bfs_q.append(State(list(state.f), state.e + 1,
                                   state.s + 1, state))
                state.f[jdx] -= 1
                state.f[idx] -= 1
                nodes += 2
    else:
        print "Unable to solve. :("


if __name__ == '__main__':
    bfs(part1, 1, True)
    bfs(part2, 2, True)