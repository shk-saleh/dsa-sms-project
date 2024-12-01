#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Comparator function to sort edges based on weights
bool cmp(vector<int> &a, vector<int> &b) {
    return a[2] < b[2];
}

// Function to initialize parent and rank for union-find
void makeSet(vector<int> &parent, vector<int> &rank, int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

// Function to find the parent of a node
int findParent(vector<int> &parent, int node) {
    if (parent[node] == node) {
        return node;
    }
    return parent[node] = findParent(parent, parent[node]); // Path compression
}

// Function to perform union of two sets
void unionSet(int u, int v, vector<int> &parent, vector<int> &rank) {
    u = findParent(parent, u);
    v = findParent(parent, v);

    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[v] < rank[u]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    }
}

// Function to calculate the minimum spanning tree using Kruskal's algorithm
int minimumSpanningTree(vector<vector<int>> &edges, int n) {
    // Sorting edges based on weights
    sort(edges.begin(), edges.end(), cmp);
    vector<int> parent(n);
    vector<int> rank(n);
    makeSet(parent, rank, n);

    int miniWeight = 0; // To store the total weight of the MST
    for (int i = 0; i < edges.size(); i++) {
        int u = findParent(parent, edges[i][0]);
        int v = findParent(parent, edges[i][1]);
        int weight = edges[i][2]; // Access the weight of the edge
        if (u != v) { // If u and v are not in the same set
            miniWeight += weight; // Add weight to MST (unchanged for unweighted)
            unionSet(u, v, parent, rank);
        }
    }
    return miniWeight;
}

int main() {
    // Example input: unweighted graph with 6 nodes (0-based indexing)
    int n = 6; // Number of nodes
    vector<vector<int>> edges = {
        {0, 1, 1}, // Edge between 0 and 1 with assumed weight 1 (change for unweighted)
        {0, 3, 1}, // Edge between 0 and 3 with assumed weight 1 (change for unweighted)
        {0, 4, 1}, // Edge between 0 and 4 with assumed weight 1 (change for unweighted)
        {3, 4, 1}, // Edge between 3 and 4 with assumed weight 1 (change for unweighted)
        {3, 2, 1}, // Edge between 3 and 2 with assumed weight 1 (change for unweighted)
        {1, 3, 1}, // Edge between 1 and 3 with assumed weight 1 (change for unweighted)
        {1, 2, 1}, // Edge between 1 and 2 with assumed weight 1 (change for unweighted)
        {1, 5, 1}, // Edge between 1 and 5 with assumed weight 1 (change for unweighted)
        {2, 5, 1}  // Edge between 2 and 5 with assumed weight 1 (change for unweighted)
    };

    // Calculate and print the weight of the minimum spanning tree
    int result = minimumSpanningTree(edges, n);
    cout << "The weight of the Minimum Spanning Tree is: " << result << endl;

    return 0;
}