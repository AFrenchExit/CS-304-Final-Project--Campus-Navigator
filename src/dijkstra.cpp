// Brandon and Sahand
// dijkstra.cpp
// this is the implementation of dijkstra

#include "dijkstra.h"
#include <iostream>

// got inspo for the logic from gfg dijkstras: (https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/)
vector<string> findShortestPath(Graph& g, string start, string end) {
    vector<string> path;
    
    // g.hasBuilding checks if the name exists
    if (!g.hasBuilding(start) || !g.hasBuilding(end)) {
        cout << "Error: One or both buildings don't exist.\n";
        return path; 
    }

    vector<string> all = g.getAllBuildings();
    int n = all.size();

    // track the distances and visited status. -1 means infinity
    vector<double> dist(n, -1.0);
    vector<bool> vis(n, false);
    vector<int> prev(n, -1);

    // find the index of the start building
    int startIdx = -1;
    for (int i = 0; i < n; i++) {
        if (all[i] == start) {
            startIdx = i;
            break;
        }
    }

    // distance to the start is 0
    dist[startIdx] = 0.0;

    // go through and loop all buildings
    for (int i = 0; i < n; i++) {
        int cur = -1;
        double minD = -1.0;

        for (int j = 0; j < n; j++) {
            // if not visited and has a distance 
            if (!vis[j] && dist[j] >= 0) {
                if (minD < 0 || dist[j] < minD) {
                    minD = dist[j];
                    cur = j;
                }
            }
        }

        // if we reached the end we can stop early
        if (all[cur] == end) break;
        vis[cur] = true;

        // check all neighbors of the current building
        vector<Edge> edges = g.getNeighbors(all[cur]);
        
        for (int j = 0; j < edges.size(); j++) {
            string nName = edges[j].destination;
            double eDist = edges[j].distance;

            // find the index of this neighbor in our list
            int nIdx = -1;
            for (int k = 0; k < n; k++) {
                if (all[k] == nName) {
                    nIdx = k;
                    break;
                }
            }

            // if the neighbor hasn't been visited yet
            if (!vis[nIdx]) {
                double newD = dist[cur] + eDist;
                
                // if we found a shorter path to the neighbor we need to update it
                if (dist[nIdx] < 0 || newD < dist[nIdx]) {
                    dist[nIdx] = newD;
                    prev[nIdx] = cur;
                }
            }
        }
    }

    // build the path by working backwards from the end
    int endIdx = -1;
    for (int i = 0; i < n; i++) {
        if (all[i] == end) {
            endIdx = i;
            break;
        }
    }

    int step = endIdx;
    while (step != startIdx) {
        path.push_back(all[step]);
        step = prev[step];
    }
    path.push_back(start);

    // reverse the path so it goes from start to end
    vector<string> finalP;
    for (int i = path.size() - 1; i >= 0; i--) {
        finalP.push_back(path[i]);
    }

    return finalP;
}