#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Comparator function to sort edges based on weights
bool cmp(vector<int> &a, vector<int> &b) {
    return a[2] < b[2];
}

// Function to initialize parent and rank for union-find
void makeSet(vector<int> &parent, vector<int> &rank, int n) 
{                                                            // n is the number of vertices
    for (int i = 0; i < n; i++)                            
    {
        parent[i] = i;
        rank[i] = 0;
    }
}

// Function to find the parent of a node
int findParent(vector<int> &parent, int node) {
    if (parent[node] == node) {
        return node;
    }
    return parent[node] = findParent(parent, parent[node]);    //recursion
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
    sort(edges.begin(), edges.end(), cmp);
    vector<int> parent(n);
    vector<int> rank(n);
    makeSet(parent, rank, n);

    int miniWeight = 0;
    for (int i = 0; i < edges.size(); i++) {
        int u = findParent(parent, edges[i][0]);
        int v = findParent(parent, edges[i][1]);
        int weight = edges[i][2];
        if (u != v) {
            miniWeight += weight;
            unionSet(u, v, parent, rank);
        }
    }
    return miniWeight;
}

int main() {
    // Example input: graph with 6 nodes (adjusted to 0-based indexing)
    int n = 6; // Number of nodes
    vector<vector<int>> edges = {
        {0, 1, 2}, // Adjusted from {1, 2, 2} 
        {0, 3, 1}, // Adjusted from {1, 4, 1}
        {0, 4, 4}, // Adjusted from {1, 5, 4}
        {3, 4, 9}, // Adjusted from {4, 5, 9}
        {3, 2, 5}, // Adjusted from {4, 3, 5}
        {1, 3, 3}, // Adjusted from {2, 4, 3}
        {1, 2, 3}, // Adjusted from {2, 3, 3}
        {1, 5, 7}, // Adjusted from {2, 6, 7}
        {2, 5, 8}  // Adjusted from {3, 6, 8}
    };

    // Calculate and print the weight of the minimum spanning tree
    int result = minimumSpanningTree(edges, n);
    cout << "The weight of the Minimum Spanning Tree is: " << result << endl;

    return 0;
}