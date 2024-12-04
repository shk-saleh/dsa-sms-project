#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <climits>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

const int MAP_ROW = 20;
const int MAP_COL = 20;

// Inventory Node Class
class Inventory {

public:
    
    int rewardId;
    char rewardName;
    int rewardScore;
    Inventory* left;
    Inventory* right;

    Inventory(int rewardId, char rewardName, int rewardScore) {
        this->rewardId = rewardId;
        this->rewardName = rewardName;
        this->rewardScore = rewardScore;
        this->left = NULL;
        this->right = NULL;
    }

};

// Integrated Game Class
class GamePathfinder {

private:

    Inventory* root;
    char matrix[MAP_ROW][MAP_COL];
    int adjMatrix[MAP_ROW][MAP_COL];

    // Inventory Management Methods
    Inventory* buildTree(Inventory* root, int rewardId, char rewardName, int rewardScore) {
        if (root == NULL) {
            root = new Inventory(rewardId, rewardName, rewardScore);
            return root;
        }

        if (root->rewardId == rewardId) {
            rewardId = rand() % 201;
            return buildTree(root, rewardId, rewardName, rewardScore);
        }

        if (root->rewardId > rewardId) {
            root->left = buildTree(root->left, rewardId, rewardName, rewardScore);
        } else {
            root->right = buildTree(root->right, rewardId, rewardName, rewardScore);
        }

        return root;
    }

    Inventory* minValue(Inventory* root) {
        Inventory* temp = root;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        return temp;
    }

    Inventory* deleteNode(Inventory* root, char rewardName, bool &isDeleted) {
            
        if (root == NULL || isDeleted) return root;

        if (root->rewardName == rewardName) {
            isDeleted = true;

            // Node deletion logic remains the same as previous implementation
            if (root->left == NULL && root->right == NULL) {
                delete root;
                return NULL;
            }
            
            if (root->left != NULL && root->right == NULL) {
                Inventory* temp = root->left;
                delete root;
                return temp;
            }

            if (root->left == NULL && root->right != NULL) {
                Inventory* temp = root->right;
                delete root;
                return temp;
            }

            if (root->left != NULL && root->right != NULL) {
                int mini = minValue(root->right)->rewardId;
                root->rewardId = mini;
                root->right = deleteNode(root->right, rewardName, isDeleted);
                return root;
            }
        }
        
        root->left = deleteNode(root->left, rewardName, isDeleted);
        root->right = deleteNode(root->right, rewardName, isDeleted);
        
        return root;
    }

    void printInventory() {


        queue<Inventory*> q;
        q.push(root);
        q.push(NULL);
        int totalScore = 0, jewel = 0, potion = 0, weapon = 0, knife = 0;

        while (!q.empty()) {
            Inventory* temp = q.front();
            q.pop();

            if (temp == NULL) {
                if (!q.empty()) {
                    q.push(NULL);
                }
            } else {
                totalScore += temp->rewardScore;

                if (temp->rewardName == 'P') potion++;
                if (temp->rewardName == 'J') jewel++;
                if (temp->rewardName == 'W') weapon++;
                if (temp->rewardName == 'K') knife++;

                if (temp->left) q.push(temp->left);
                if (temp->right) q.push(temp->right);
            }
        }

        cout<< "You have collected: " << jewel << " Jewel, " 
        << potion << " Potion, " << weapon << " Weapon, "<< knife << " knifes."<<endl
        << "Total score: " << totalScore << endl;

    }

    vector<pair<int, int>> dijkstra(int srcX, int srcY, int destX, int destY) {
       
        vector<vector<int>> dist(MAP_ROW, vector<int>(MAP_COL, INT_MAX));
        vector<vector<pair<int, int>>> parent(MAP_ROW, vector<pair<int, int>>(MAP_COL, {-1, -1}));
        set<pair<int, pair<int, int>>> st;

        dist[srcX][srcY] = 0;
        st.insert({0, {srcX, srcY}});

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        while (!st.empty()) {
            auto front = *(st.begin());
            st.erase(st.begin());

            int nodeDist = front.first;
            int x = front.second.first;
            int y = front.second.second;

            if (x == destX && y == destY) break;

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < MAP_ROW && ny >= 0 && ny < MAP_COL) {
                    int weight = adjMatrix[nx][ny];
                    if (nodeDist + weight < dist[nx][ny]) {
                        st.erase({dist[nx][ny], {nx, ny}});
                        dist[nx][ny] = nodeDist + weight;
                        parent[nx][ny] = {x, y};
                        st.insert({dist[nx][ny], {nx, ny}});

                        // Reward and Obstacle Handling
                        handleMapInteraction(nx, ny);
                    }
                }
            }
        }

        vector<pair<int, int>> path;
        int x = destX, y = destY;
        while (x != -1 && y != -1) {
            path.push_back({x, y});
            tie(x, y) = parent[x][y];
        }

        reverse(path.begin(), path.end());
        return path;
    }

    // functions to handle map objects 
    void handleMapInteraction(int x, int y) {
        switch (matrix[x][y]) {
            case 'J':
                // cout<<"You have collected Jewel! +50"<<endl;
                root = buildTree(root, rand() % 201, 'J', 50);
                break;
            case 'W':
                // cout<<"You have collected Weapon! +60"<<endl;
                root = buildTree(root, rand() % 201, 'W', 60);
                break;
            case 'P':
                // cout<<"You have collected Potion! +70"<<endl;
                root = buildTree(root, rand() % 201, 'P', 70);
                break;
            case '@':
            case '$':
            case '&': {
                bool isDeleted = false;
                char rewardToDelete = (matrix[x][y] == '@') ? 'W' : (matrix[x][y] == '$') ? 'P' : 'J';
                deleteNode(root, rewardToDelete, isDeleted);
                break;
            }
            case '*':
                cout << "You found the crystal! Congratulations!" << endl;
                break;
            
            case 'K':
                // cout<<"You have collected Knife! +30"<<endl;
                root = buildTree(root, rand() % 201, 'K', 30);
                break;

        }
    }


public:

    GamePathfinder() {

        srand(time(0));
        root = NULL;
        
        // Sample Matrix Setup
        char gameMap[MAP_ROW][MAP_COL] = {
                            {'H', 'P', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'J', '#', 'C', 'C', 'C', 'C', 'C', 'K', 'C', 'C', 'C'},
                            {'C', 'C', '$', 'C', 'C', 'C', 'K', 'C', 'C', 'C', 'J', '#', 'W', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', '#', 'J', 'C', 'C', 'C', 'W', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C'},
                            {'K', '#', 'P', 'C', 'C', 'C', '#', 'C', 'C', 'J', 'C', 'C', 'C', 'C', 'C', '$', 'K', '@', 'C', '&'},
                            {'C', 'C', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', 'K', 'C', '#', 'C', 'J', 'C', 'C', 'C', 'C'},
                            {'C', 'C', 'K', '-', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', '#', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', 'C', 'C', '$', '&', 'P', 'J', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', '&', '$', 'K', 'C', '@'},
                            {'C', 'C', 'C', 'C', '#', 'W', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '&', 'S', 'W', 'C', 'K', 'C'},
                            {'C', 'C', '@', 'C', 'C', 'J', 'C', 'W', 'C', 'C', 'W', 'C', '#', 'C', 'C', '#', 'C', 'K', 'C', 'C'},
                            {'C', 'J', 'C', 'C', 'C', 'C', 'C', 'P', 'C', '#', '&', '$', 'W', 'C', 'K', 'C', 'C', 'C', 'C', 'C'},
                            {'K', 'C', '&', 'C', '#', '#', 'C', 'C', 'C', 'K', 'C', 'C', 'C', '#', '#', 'C', 'J', 'C', 'C', 'C'},
                            {'C', 'P', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'K', 'C', '#', 'C', 'C', 'C', '@', 'C'},
                            {'C', 'C', 'K', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', '#', '#', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', '#', 'C', '#', '@', '#', 'K', 'C', '@', 'C'},
                            {'#', 'C', 'C', '@', 'C', 'C', 'C', '#', 'C', 'C', '$', 'C', '@', 'C', 'C', 'P', 'C', 'K', 'C', '@'},
                            {'C', 'K', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'W', 'C', 'C'},
                            {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'K', 'C', 'W', 'C', 'C', '$', 'C', 'C', 'C', 'C'},
                            {'C', '#', '*', 'C', 'C', 'W', 'C', 'K', 'C', 'C', 'C', '$', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C'},
                            {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'#', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'J', 'C', 'C', 'C', 'C', '*'}
            };
 
        memcpy(matrix, gameMap, sizeof(gameMap));

        // Create Adjacency Matrix
        for (int i = 0; i < MAP_ROW; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                adjMatrix[i][j] = (matrix[i][j] == '#') ? 100 : 1;
            }
        }
    }

    void runGame() {

        vector<pair<int, int>> path = dijkstra(0, 0, 19, 19);

        cout << "Shortest Path Coordinates:\n";
        for (auto p : path) {
            cout << "(" << p.first << ", " << p.second << ")\n";
        }

        printInventory();
    }

};

int main() {

    GamePathfinder game;
    game.runGame();
    return 0;

}