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

// const int MAP_ROW = 20;
// const int MAP_COL = 20;
const int ROWS = 20;
const int COLS = 20;
const int INF = INT_MAX;


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


// Dijistra class - collecting rewards and finding shortest route
class Dijistra{

    private:

    Inventory* root;
    char matrix[ROWS][COLS];
    int adjMatrix[ROWS][COLS];

    // Inventory Management Methods
    Inventory* buildTree(Inventory* root, int rewardId, char rewardName, int rewardScore) {
       
        if (root == NULL) {
            root = new Inventory(rewardId, rewardName, rewardScore);
            return root;
        }

        if (root->rewardId == rewardId) {           // if any node exits with same id then it will assign a new 
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

    // helper function for deletion
    Inventory* minValue(Inventory* root) {
        Inventory* temp = root;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        return temp;
    }

    // deletion of any reward
    Inventory* deleteNode(Inventory* root, char rewardName, bool &isDeleted) {
            
        if (root == NULL || isDeleted) return root;     // if root null already 

        if (root->rewardName == rewardName) {
           
            isDeleted = true;   // marks true if that node get deleted 
            
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

    // refersh the tree - by wiping it whole
    void deleteTree(Inventory*& root) {

        if (root == NULL) return;

        // Recursively delete left and right subtrees
        deleteTree(root->left);
        deleteTree(root->right);

        // Delete the current node
        delete root;
        
        root = NULL; 

    }


    // print the reward inventory - Uses level order traversal
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

                // if temp -> left or right are non-null then do these 
                if (temp->left) q.push(temp->left);
                if (temp->right) q.push(temp->right);
            }
        }

        cout<< "You have collected: " << jewel << " Jewel, " 
        << potion << " Potion, " << weapon << " Weapon, "<< knife << " knifes."<<endl
        << "Total score: " << totalScore << endl;

    }


    // dijistra algo
    vector<pair<int, int>> dijkstra(int srcX, int srcY, int destX, int destY) {
       
        vector<vector<int>> dist(ROWS, vector<int>(COLS, INT_MAX));
        vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));
        set<pair<int, pair<int, int>>> st;
 
        dist[srcX][srcY] = 0;       // init dist 0 -> 0 is 0
        st.insert({0, {srcX, srcY}});

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        while (!st.empty()) {

            auto front = *(st.begin());     // get front node 
            st.erase(st.begin());           // del that node

            int nodeDist = front.first;     
            int x = front.second.first;
            int y = front.second.second;

            if (x == destX && y == destY) break;    // if we reach the destination coordinates

            for (int i = 0; i < 4; i++) {   // we have to check all 4 sides - left/right/bottom/top
                
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS) {
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

        // return the final path - backtraking 
        vector<pair<int, int>> path;
        int x = destX, y = destY;
        while (x != -1 && y != -1) {        // until we not reach to src
            path.push_back({x, y});
            tie(x, y) = parent[x][y];       // tie - make pair of coordinates to return 
        }

        reverse(path.begin(), path.end());
        return path;
    }

    // functions to handle map objects 
    void handleMapInteraction(int x, int y) {
        switch (matrix[x][y]) {
            case 'J':
                root = buildTree(root, rand() % 201, 'J', 50);
                break;
            case 'W':
                root = buildTree(root, rand() % 201, 'W', 60);
                break;
            case 'P':
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
           
            case 'K':
                root = buildTree(root, rand() % 201, 'K', 30);
                break;

            case '*':
                cout << "You found the crystal! Congratulations!" << endl;
                break;

        }
    }


    public:


    // constructor
    Dijistra() {

        srand(time(0));
        root = NULL;
        
        // init game map matrix
        char gameMap[ROWS][COLS] = {
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
 
        memcpy(matrix, gameMap, sizeof(gameMap));       // created a 2d array from gameMap 

        // Create Adjacency Matrix
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                adjMatrix[i][j] = (gameMap[i][j] == '#') ? 100 : 1;
            }
        }
    }

    void play_with_dijistra() {

        deleteTree(root);   // refresh the tree before playing again

        vector<pair<int, int>> path = dijkstra(0, 0, 19, 19);

        cout << "Shortest Path Coordinates:\n";
        for (auto p : path) {
            cout << "(" << p.first << ", " << p.second << ")\n";
        }

        printInventory();
    }

};



// prims algo class
class Prims{


    public:

    int weights[ROWS][COLS]; 
    bool visited[ROWS][COLS]; 

    char matrix[ROWS][COLS] = {
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



    // init weight for each node
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

    // find pair of that cordinated that has lesser weight
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

        return {min_row, min_col};          // return in the form of pair
    }


    // main logic for prism algo
    void prims_algorithm() {

        init_weights(); 
        
        int total_score = 0;
        int parent[ROWS][COLS];

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                parent[row][col] = -1; // init parent as -1 - in start we don't now any parent 
            }
        }
        

        // started traversing the graph
        for (int i = 0; i < ROWS * COLS; i++) {
            
            auto [curr_row, curr_col] = min_weight_node();      // return the pair of coordinates
            
            if (curr_row == -1 || curr_col == -1) break;        // if not valid

            visited[curr_row][curr_col] = true;         // as visited it mark that node matrix box true
            total_score += weights[curr_row][curr_col];  // calculate the total weight of min spanning tree

            if (matrix[curr_row][curr_col] == '*') { 

                cout << "\nThe minimum weight to reach the end node is : " << total_score << endl<<endl;
                
                //printing edges of MST / coordinates
                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++) {
                        if (parent[row][col] != -1) {
                            int parent_row = parent[row][col] / COLS;
                            int parent_col = parent[row][col] % COLS;
                            cout <<"Coordinates : "<<"(" << row << "," << col << ") -- (" << parent_row << "," << parent_col << ")" << endl;
                        }
                    }
                }

                return;
            }
            
            //update weights and parent adj nodes
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



// main function
int main()
{

    Dijistra dj;
    Prims p;
	int choice;


	do {
		cout << "\t\t\t\t\t========================================" << endl;
		cout << "\t\t\t\t\t\t Quest In a Forest Game " << endl;
		cout << "\t\t\t\t\t========================================" << endl;
		cout << endl;

		cout << "\t\t\t\t\t-----------------------------------------" << endl;
		cout << "\t\t\t\t\t|1. Play Game                           |" << endl;
		cout << "\t\t\t\t\t|2. How To Play                         |" << endl;
		cout << "\t\t\t\t\t|3. Exit                                |" << endl;
		cout << "\t\t\t\t\t-----------------------------------------" << endl;
		cout << endl;

		cout << "Enter your choice: " << " ";
		
		cin >> choice;

		switch (choice) {
		
		case 1:

			cout << endl;
			cout << "Following are the algorithms to find the path: " << endl;
			cout << "a. Dijistra Algorithm " << endl;
			cout << "b. Prims Algorithm " << endl;
			cout << endl;
			cout << "Enter the algorithm you want to use -> " << " ";
			char choice;
			cin >> choice;

			if (choice == 'a') {
                // play game with dijistra
				dj.play_with_dijistra();
			}
			else if (choice == 'b') {
				// Implement the prims algorithm 
				p.prims_algorithm();
			}
			else {
				cout << "Invalid Entry " << endl;
			}

			break;

		case 2:

			cout << endl;
			cout << "\t\t-----------------------------------------------------------------------------------------------------------" << endl;
			cout << "\t\t\t\t\t\t -----------------" <<  endl;
			cout << "\t\t\t\t\t\t|Plot of the game |" << endl;
			cout << "\t\t\t\t\t\t -----------------" <<  endl;
			cout << "\t\t\t->The player is on a quest to retrieve a mystical crystal from dangerous and enchanted forest." << endl;
			cout << "\t\t\tThe forest is filled with mystical enemies and obstacles." << endl;
			cout << "\t\t\tThe player must cross through the forest to find that mystical crystal." << endl;
			cout << "\t\t\tThe player must cross through the forest to find that mystical crystal." << endl;
			cout << "\t\t-----------------------------------------------------------------------------------------------------------" << endl;
			cout << endl;

			break;

		case 3:
		
			cout << "Exited from the game" << endl;
			break;

		default:
			cout << "Invalid Choice " << endl;
		}
		
	} while (choice!=3);

}
