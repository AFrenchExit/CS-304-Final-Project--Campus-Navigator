// test_graph.cpp
// a simple way to test to prove the graph loads correctly. too be changed if needed for certain instances 

#include "graph.h"
#include <iostream>

using namespace std;

int main() {
    Graph campus;
    
    // load the data 
    // given from OSRM:
    if (campus.loadFromJson("../data/buildings.json", "../data/edges.json") == false) {
        return 1;
    }

    string target = "John C. Hodges Library";
    cout << "\nNeighbors of " << target << ":\n";
    
    vector<Edge> neighbors = campus.getNeighbors(target);
    
    for (int i = 0; i < neighbors.size(); i++) {
        cout << "  -> " << neighbors[i].destination << " (" << neighbors[i].distance << " meters)\n";
    }

    return 0;
}