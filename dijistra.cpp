#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <algorithm>
using namespace std;



const int MAP_ROW = 20;
const int MAP_COL = 20;

vector<pair<int, int>> dijkstra(int adjMatrix[MAP_ROW][MAP_COL], int srcX, int srcY, int destX, int destY) {

    vector<vector<int>> dist(MAP_ROW, vector<int>(MAP_COL, INT_MAX));
    vector<vector<pair<int, int>>> parent(MAP_ROW, vector<pair<int, int>>(MAP_COL, {-1, -1}));
    set<pair<int, pair<int, int>>> st; // {distance, {x, y}}

    dist[srcX][srcY] = 0;
    st.insert({0, {srcX, srcY}});

    int dx[] = {-1, 1, 0, 0}; // Directions: up, down, left, right
    int dy[] = {0, 0, -1, 1};

    while (!st.empty()) {

        auto front = *(st.begin());
        st.erase(st.begin());

        int nodeDist = front.first;
        int x = front.second.first;
        int y = front.second.second;

        if (x == destX && y == destY) 
            break;      // if we find the dest then stop 

        for (int i = 0; i < 4; i++) { // Explore neighbors
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < MAP_ROW && ny >= 0 && ny < MAP_COL) { // Check boundaries
                int weight = adjMatrix[nx][ny];
                if (nodeDist + weight < dist[nx][ny]) {
                    st.erase({dist[nx][ny], {nx, ny}});
                    dist[nx][ny] = nodeDist + weight;
                    parent[nx][ny] = {x, y};
                    st.insert({dist[nx][ny], {nx, ny}});
                }
            }
        }
    }

    // Backtraking the parents dest to src 
    vector<pair<int, int>> path;
    int x = destX, y = destY;       // store dest x coordinate or y coordinate
    while (x != -1 && y != -1) {    // unitl we reach the src node 
        path.push_back({x, y});
        tie(x, y) = parent[x][y];
    }

    reverse(path.begin(), path.end()); // Reverse to get path from source to destination
   
    return path;
}

int main() {

    char matrix[MAP_ROW][MAP_COL] = {
        {'H', 'P', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'J', '#', 'C', 'C', 'C', 'C', 'C', '%', 'C', 'C', 'C'},
        {'C', 'C', '$', 'C', 'C', 'C', '%', 'C', 'C', 'C', 'J', '#', 'W', 'C', 'C', 'C', 'C', 'C', 'C', '#'},
        {'C', '#', 'C', 'C', 'C', 'C', 'W', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C'},
        {'%', '#', 'P', 'C', 'C', 'C', '#', 'C', 'C', 'J', 'C', 'C', 'C', 'C', 'C', '$', '%', '@', 'C', '&'},
        {'C', 'C', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', '%', 'C', '#', 'C', 'J', 'C', 'C', 'C', 'C'},
        {'C', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', '#', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
        {'C', 'C', 'C', '$', '&', 'P', 'J', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', '&', '$', '%', 'C', '@'},
        {'C', 'C', 'C', 'C', '#', 'W', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '&', 'S', 'W', 'C', '%', 'C'},
        {'C', 'C', '@', 'C', 'C', 'J', 'C', 'W', 'C', 'C', 'W', 'C', '#', 'C', 'C', '#', 'C', '%', 'C', 'C'},
        {'C', 'J', 'C', 'C', 'C', 'C', 'C', 'P', 'C', '#', '&', '$', 'W', 'C', '%', 'C', 'C', 'C', 'C', 'C'},
        {'%', 'C', '&', 'C', '#', '#', 'C', 'C', 'C', '%', 'C', 'C', 'C', '#', '#', 'C', 'J', 'C', 'C', '%'},
        {'C', 'P', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', 'C', '%', 'C', '#', 'C', 'C', 'C', '@', 'C'},
        {'C', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
        {'C', '#', '#', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', '#', 'C', '#', '@', '#', '%', 'C', '@', 'C'},
        {'#', 'C', 'C', '@', 'C', 'C', 'C', '#', 'C', 'C', '$', 'C', '@', 'C', 'C', 'P', 'C', '%', 'C', '@'},
        {'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'W', 'C', 'C'},
        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '%', 'C', 'W', 'C', 'C', '$', 'C', 'C', 'C', 'C'},
        {'C', '#', '%', 'C', 'C', 'W', 'C', '%', 'C', 'C', 'C', '$', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C'},
        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
        {'#', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'J', 'C', 'C', 'C', 'C', '*'}
    };

    int adjMatrix[MAP_ROW][MAP_COL];
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            adjMatrix[i][j] = (matrix[i][j] == '#') ? 100 : 1;      // add weight of 100 to obstactle otherwise weight of 1
        }
    }

    vector<pair<int, int>> path = dijkstra(adjMatrix, 0, 0, MAP_ROW - 1, MAP_COL - 1);

    cout << "Shortest Path Coordinates:\n";
    for (auto p : path) {
        cout << "(" << p.first << ", " << p.second << ")\n";
    }

    return 0;
}
