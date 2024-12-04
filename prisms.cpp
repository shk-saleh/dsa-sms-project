#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;


const int ROWS = 20;
const int COLS = 20;
const int INF = INT_MAX;

class Prism{


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
        
        auto [curr_row, curr_col] = min_weight_node();
        
        if (curr_row == -1 || curr_col == -1) break;        // if not valid

        visited[curr_row][curr_col] = true;         // as visited it mark that node matrix box true
        total_score += weights[curr_row][curr_col];  // calculate the total weight of min spanning tree

        if (matrix[curr_row][curr_col] == '*') { 

            cout << "The minimum weight to reach the end node is " << total_score << endl;
            
            //printing edges / coordinates
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
        
        //update wieghts and parnt adj nodes
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



int main(){

    Prism p;

    p.prims_algorithm();

}