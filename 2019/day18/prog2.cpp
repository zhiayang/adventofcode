// https://github.com/kaiskye/adventofcode-2019/blob/master/day18.cpp

#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Pos {
    int row;
    int col;
};

struct State {
    std::vector<Pos> robots;
    std::uint32_t keys;
};

bool operator==(const Pos& a, const Pos& b) {
    return a.row == b.row && a.col == b.col;
}

bool operator==(const State& a, const State& b) {
    return a.robots == b.robots && a.keys == b.keys;
}

template <class T>
void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std {
    template<>
    struct hash<State> {
        std::size_t operator()(const State& s) const {
            std::size_t seed = 0;
            for (Pos p : s.robots) {
                hash_combine(seed, p.row);
                hash_combine(seed, p.col);
            }
            hash_combine(seed, s.keys);
            return seed;
        }
    };
}

State grid_state(const std::vector<std::vector<char>>& grid) {
    const int height = static_cast<int>(grid.size());
    const int width = static_cast<int>(grid[0].size());
    State state = {};
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            const char c = grid[row][col];
            if (c == '@') {
                state.robots.push_back({row, col});
            }
            if (std::islower(c)) {
                state.keys |= 1 << (c - 'a');
            }
        }
    }
    return state;
}

std::vector<std::pair<State, int>> find_reachable_keys(const std::vector<std::vector<char>>& grid, const State& cur, int robot) {
    std::vector<std::pair<State, int>> result;
    std::queue<State> q;
    std::unordered_set<State> seen;
    q.push(cur);
    seen.insert(cur);
    int steps = 1;
    while (!q.empty()) {
        const int size = static_cast<int>(q.size());
        for (int i = 0; i < size; i++) {
            const State s = q.front(); q.pop();
            for (int d = 0; d < 4; d++) {
                const int d_row[] = {-1, 0, 1, 0};
                const int d_col[] = {0, -1, 0, 1};
                Pos next_pos;
                next_pos.row = s.robots[robot].row + d_row[d];
                next_pos.col = s.robots[robot].col + d_col[d];
                const char c = grid[next_pos.row][next_pos.col];
                if (c == '#') {
                    continue;
                }
                if (std::isupper(c) && (s.keys & (1 << (std::tolower(c) - 'a'))) != 0) {
                    continue;
                }
                State next_state = s;
                next_state.robots[robot] = next_pos;
                const bool found_key = std::islower(c) && (s.keys & (1 << (c - 'a'))) != 0;
                if (found_key) {
                    next_state.keys &= ~(1 << (c - 'a'));
                }
                auto [_, inserted] = seen.insert(next_state);
                if (inserted) {
                    if (found_key) {
                        result.push_back({next_state, steps});
                    } else {
                        q.push(next_state);
                    }
                }
            }
        }
        steps++;
    }
    return result;
}

int dijkstras(const std::vector<std::vector<char>>& grid) {
    std::unordered_map<State, int> dist;
    auto cmp = [&](const State& a, const State& b) {
        return dist[a] > dist[b];
    };

    std::unordered_set<State> seen;
    std::priority_queue<State, std::vector<State>, decltype(cmp)> q(cmp);

    State initial = grid_state(grid);
    q.push(initial);
    seen.insert(initial);
    dist[initial] = 0;

    while (!q.empty()) {
        const int size = static_cast<int>(q.size());
        for (int i = 0; i < size; i++) {
            const State s = q.top(); q.pop();
            if (s.keys == 0) {
                return dist[s];
            }
            for (int r = 0; r < static_cast<int>(s.robots.size()); r++) {
                std::vector<std::pair<State, int>> next_states = find_reachable_keys(grid, s, r);
                for (const auto& [neighbor, steps] : next_states) {
                    const int new_dist = dist[s] + steps;
                    auto [_, inserted] = seen.insert(neighbor);
                    if (inserted || new_dist < dist[neighbor]) {
                        dist[neighbor] = new_dist;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    return -1;
}

std::vector<std::vector<char>> read_input(const char* filename) {
    std::ifstream f(filename);
    if (!f) {
        return {};
    }
    std::vector<std::vector<char>> grid;
    std::string line;
    while (std::getline(f, line)) {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(std::move(row));
    }
    return grid;
}

int main() {
    const auto grid = read_input("input2.txt");
    if (grid.empty()) {
        return 1;
    }
    const int steps = dijkstras(grid);
    std::cout << steps << std::endl;
    return 0;
}
