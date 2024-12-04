#include <iostream>
#include <vector>
#include <climits>
using namespace std;

const int ROWS = 10;
const int COLS = 10;
const int INF = INT_MAX;

class Prism {
public:
    int weights[ROWS][COLS];
    bool visited[ROWS][COLS];
    char matrix[ROWS][COLS] = {
        {'C', 'C', 'C', 'x', 'x', 'Z', 'C', '&', 'P', '#'},
        {'C', 'C', '%', 'C', 'C', 'C', 'C', '%', 'C', 'C'},
        {'C', 'C', 'C', 'C', 'C', '%', 'C', '%', 'C', 'C'},
        {'C', 'C', 'C', 'C', '%', 'C', 'C', 'C', 'C', 'C'},
        {'C', '#', '#', 'C', '#', 'C', 'J', '%', 'C', 'W'},
        {'C', 'C', 'C', 'P', '#', 'C', 'C', '#', '%', '#'},
        {'C', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
        {'C', 'C', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C'},
        {'%', 'C', 'C', '%', 'C', 'C', '@', '*', 'P', 'C'},
        {'C', 'C', 'C', '@', '#', 'C', '#', 'C', 'W', '#'}
    };

    void init_weights() {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                visited[row][col] = false;
                if (matrix[row][col] == '@' || matrix[row][col] == '&' || matrix[row][col] == '#' || matrix[row][col] == '%')
                    weights[row][col] = INF;
                else if (matrix[row][col] == 'J')
                    weights[row][col] = 30;
                else if (matrix[row][col] == 'W')
                    weights[row][col] = 50;
                else if (matrix[row][col] == 'P')
                    weights[row][col] = 70;
                else
                    weights[row][col] = 1;
            }
        }
    }

    pair<int, int> min_weight_node() {
        int min_weight = INF;
        int min_row = -1, min_col = -1;

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (!visited[row][col] && weights[row][col] < min_weight) {
                    min_weight = weights[row][col];
                    min_row = row;
                    min_col = col;
                }
            }
        }

        return {min_row, min_col};
    }

    void prims_algorithm() {
        init_weights();

        int total_score = 0;
        int parent[ROWS][COLS];
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                parent[row][col] = -1;
            }
        }

        for (int i = 0; i < ROWS * COLS; i++) {
            auto [curr_row, curr_col] = min_weight_node();
            if (curr_row == -1 || curr_col == -1) break;

            visited[curr_row][curr_col] = true;
            total_score += weights[curr_row][curr_col];

            if (matrix[curr_row][curr_col] == '*') {
                cout << "Minimum weight to reach the end node: " << total_score << endl;

                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++) {
                        if (parent[row][col] != -1) {
                            int parent_row = parent[row][col] / COLS;
                            int parent_col = parent[row][col] % COLS;
                            cout << "Coordinates: (" << row << "," << col << ") -- (" << parent_row << "," << parent_col << ")" << endl;
                        }
                    }
                }
                return;
            }

            vector<pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
            for (auto [dr, dc] : directions) {
                int adj_row = curr_row + dr;
                int adj_col = curr_col + dc;
                if (adj_row >= 0 && adj_row < ROWS && adj_col >= 0 && adj_col < COLS) {
                    if (!visited[adj_row][adj_col] && weights[adj_row][adj_col] > weights[curr_row][curr_col]) {
                        weights[adj_row][adj_col] = weights[curr_row][curr_col];
                        parent[adj_row][adj_col] = curr_row * COLS + curr_col;
                    }
                }
            }
        }
    }
};

int main() {
    Prism prism;
    prism.prims_algorithm();
    return 0;
}
