#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int MAP_SIZE = 20;

class NodeMST {
public:
    int x, y;
    char type;
    bool visited;
    NodeMST* parent;
    int score;

    NodeMST(int x = 0, int y = 0, char type = '\0') 
        : x(x), y(y), type(type), visited(false), parent(this), score(0) {}
};

class Edge {
public:
    NodeMST* start;
    NodeMST* end;
    int weight;

    Edge(NodeMST* start, NodeMST* end, int weight) 
        : start(start), end(end), weight(weight) {}
};

// Comparator for sorting edges by weight
bool compareEdges(Edge* e1, Edge* e2) {
    return e1->weight < e2->weight;
}

// Find operation with path compression
NodeMST* findParent(NodeMST* node) {
    if (node->parent != node) {
        node->parent = findParent(node->parent);
    }
    return node->parent;
}

// Union operation
void unionNodes(NodeMST* node1, NodeMST* node2) {
    NodeMST* parent1 = findParent(node1);
    NodeMST* parent2 = findParent(node2);
    parent1->parent = parent2;
}

int playerScore = 0;

// Helper function to collect rewards
void collectReward(NodeMST* node) {
    if (!node->visited) {
        switch (node->type) {
        case 'J':
            playerScore += 50;
            cout << "Collected a jewel at (" << node->x << ", " << node->y << ")! +50 points" << endl;
            break;
        case 'W':
            playerScore += 30;
            cout << "Collected a weapon at (" << node->x << ", " << node->y << ")! +30 points" << endl;
            break;
        case 'P':
            playerScore += 70;
            cout << "Collected a potion at (" << node->x << ", " << node->y << ")! +70 points" << endl;
            break;
        }
        node->visited = true;
    }
}

void kruskal(NodeMST gameMap[MAP_SIZE][MAP_SIZE]) {
    vector<Edge*> edges;

    // Build all valid edges
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            NodeMST* node = &gameMap[i][j];
            if (node->type == '#' || node->type == 'H') continue; // Skip obstacles and the start node
            
            // Right neighbor
            if (j < MAP_SIZE - 1) {
                NodeMST* neighbor = &gameMap[i][j + 1];
                if (neighbor->type != '#') {
                    int weight = (node->type == '*' || neighbor->type == '*') ? 0 : 1;
                    edges.push_back(new Edge(node, neighbor, weight));
                }
            }
            // Down neighbor
            if (i < MAP_SIZE - 1) {
                NodeMST* neighbor = &gameMap[i + 1][j];
                if (neighbor->type != '#') {
                    int weight = (node->type == '*' || neighbor->type == '*') ? 0 : 1;
                    edges.push_back(new Edge(node, neighbor, weight));
                }
            }
        }
    }

    // Sort edges by weight
    sort(edges.begin(), edges.end(), compareEdges);

    // Kruskal's MST
    cout << "Building Minimum Spanning Tree..." << endl;
    for (Edge* edge : edges) {
        NodeMST* start = edge->start;
        NodeMST* end = edge->end;

        if (findParent(start) != findParent(end)) {
            unionNodes(start, end);
            cout << "MST Edge: (" << start->x << ", " << start->y << ") -> ("
                 << end->x << ", " << end->y << ")" << endl;

            // Collect rewards
            collectReward(start);
            collectReward(end);
        }
    }

    // Clean up dynamically allocated edges
    for (Edge* edge : edges) {
        delete edge;
    }

    cout << "Player's final score: " << playerScore << endl;
}

int main() {

    vector<vector<char>> map = {
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
        {'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C'},
        {'C', 'C', '#', '%', 'C', '@', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#'},
        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C', 'C', 'C', 'C', '@'},
        {'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C'}
    };

    NodeMST gameMap[MAP_SIZE][MAP_SIZE];

    // Populate the game map
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            gameMap[i][j] = NodeMST(i, j, map[i][j]);
        }
    }

    kruskal(gameMap);

    return 0;
    
}
