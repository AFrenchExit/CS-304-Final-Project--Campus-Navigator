// test_dijkstra.cpp
// quick little test script for the dijkstra algorithm.

#include "graph.h"
#include "dijkstra.h"
#include <iostream>

using namespace std;

int main() {
    Graph campus;
    
    if (campus.loadFromJson("../data/buildings.json", "../data/edges.json") == false) {
        return 1;
    }

    // Let's try finding a path
    string start = "John C. Hodges Library";
    string end = "UT Veterinary Medical Center";
    cout << "\nFinding path from " << start << " to " << end << "...\n\n";
    
    vector<string> path = findShortestPath(campus, start, end);

    if (path.size() > 0) {
        cout << "Shortest Path:\n";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\n\n";
    }

    return 0;
}