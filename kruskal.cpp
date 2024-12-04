#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


// Edge structure for Kruskal's algorithm
class Edge {

    public:

    int u, v, weight;
    
    Edge(int u, int v, int weight){
        this -> u = u;
        this -> v = v;
        this -> weight = weight;
    }

};

// Disjoint Set Union (DSU) for Kruskal's algorithm
class DSU{

    public:

    vector<int> parent, rank;

    DSU(int n) {

        parent.resize(n);       // no of nodes in our map
        rank.resize(n, 0);

        for (int i = 0; i < n; ++i)  parent[i] = i; // initailized the parent nodes
   
    }

    int find(int x) {

        if (parent[x] != x)
            parent[x] = find(parent[x]);
        
        return parent[x];
   
    }

    bool unite(int x, int y) {

        int xr = find(x), yr = find(y);

        if (xr == yr) return false;

        if (rank[xr] < rank[yr]) 
            swap(xr, yr);
        
        parent[yr] = xr;
        
        if (rank[xr] == rank[yr]) 
            rank[xr]++;
        
        return true;
   
    }

};

// Kruskal's algorithm to find the MST
vector<Edge> kruskal(int n, vector<Edge>& edges) {
    
    sort(edges.begin(), edges.end(), [](Edge& a, Edge& b) {
        return a.weight < b.weight;
    });

    DSU dsu(n);
    vector<Edge> mst;   // find min spanning tree

    for (Edge& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            mst.push_back(edge);
        }
    }


    return mst;
}

// Game state and traversal logic
void traverseGame(vector<Edge>& mst, int start, int end, vector<char>& rewards) {

    vector<bool> visited(rewards.size(), false);

    for (Edge& edge : mst) {

        if (!visited[edge.u]) {

            cout << "Traversing node " << edge.u << endl;
            visited[edge.u] = true;

            // Handle rewards or deletions at this node
            if (rewards[edge.u] == 'J') {
                cout << "Collected Jewel at " << edge.u << endl;
            } else if (rewards[edge.u] == '%') {
                cout << "You stepped on a trap at " << edge.u << "! Restarting..." << endl;
                return;
            }
            // Add other reward or deletion logic here...
        }
    }

    cout << "Traversal complete! Reached destination." << endl;
}

int main() {
    // Sample graph representation as edges
    int n = 6; // Number of nodes
    vector<Edge> edges = {
        Edge(0, 1, 1), Edge(0, 2, 1), Edge(1, 2, 100),
        Edge(1, 3, 5), Edge(2, 3, 8), Edge(3, 4, 6),
        Edge(4, 5, 9), Edge(2, 5, 10), 
    };

    // Rewards array for nodes
    vector<char> rewards = {'S', 'J', '%', 'W', 'P', '*'}; // Example rewards for nodes

    // Apply Kruskal's algorithm
    vector<Edge> mst = kruskal(n, edges);

    // Display the MST
    cout << "Edges in the MST:" << endl;
    for (Edge& edge : mst) {
        cout << edge.u << " - " << edge.v << " : " << edge.weight << endl;
    }

    // Traverse the game from start to end
    int start = 0, end = 5;
    traverseGame(mst, start, end, rewards);

    return 0;
}
