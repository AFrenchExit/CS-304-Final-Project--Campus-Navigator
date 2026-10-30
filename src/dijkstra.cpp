// Brandon and Sahand
// dijkstra.cpp
// this is the implementation of dijkstra

#include "dijkstra.h"
#include <iostream>

vector<string> findShortestPath(Graph& campus, string start, string end) {
    vector<string> path;
    
    // error check if buildings exist
    if (!campus.hasBuilding(start) || !campus.hasBuilding(end)) {
        cout << "Error: One or both buildings don't exist.\n";
        return path; 
    }

    vector<string> allBuildings = campus.getAllBuildings();
    int numBuildings = allBuildings.size();

    // track the distances and visited status. -1 means infinity
    vector<double> distances(numBuildings, -1.0);
    vector<bool> visited(numBuildings, false);
    vector<int> previous(numBuildings, -1);

    // find the index of the start building
    int startIndex = -1;
    for (int i = 0; i < numBuildings; i++) {
        if (allBuildings[i] == start) {
            startIndex = i;
            break;
        }
    }

    // distance to the start is 0
    distances[startIndex] = 0.0;

    // go through and loop all buildings
    for (int i = 0; i < numBuildings; i++) {
        int currentIndex = -1;
        double smallestDist = -1.0;

        for (int j = 0; j < numBuildings; j++) {
            // if not visited and has a distance 
            if (!visited[j] && distances[j] >= 0) {
                if (smallestDist < 0 || distances[j] < smallestDist) {
                    smallestDist = distances[j];
                    currentIndex = j;
                }
            }
        }

        // if we couldn't find a building just break
        if (currentIndex == -1) {
            break;
        }
        visited[currentIndex] = true;

        // stop if we already reached the distance. ex: hodges libary to hodges libary
        if (allBuildings[currentIndex] == end) {
            break;
        }

        // check all neighbors of the current building
        vector<Edge> neighbors = campus.getNeighbors(allBuildings[currentIndex]);
        
        for (int j = 0; j < neighbors.size(); j++) {
            string neighborName = neighbors[j].destination;
            double edgeDist = neighbors[j].distance;

            // find the index of this neighbor in our list
            int neighborIndex = -1;
            for (int k = 0; k < numBuildings; k++) {
                if (allBuildings[k] == neighborName) {
                    neighborIndex = k;
                    break;
                }
            }

            // if the neighbor hasn't been visited yet
            if (!visited[neighborIndex]) {
                double newDist = distances[currentIndex] + edgeDist;
                
                // if we found a shorter path to the neighbor we need to update it
                if (distances[neighborIndex] < 0 || newDist < distances[neighborIndex]) {
                    distances[neighborIndex] = newDist;
                    previous[neighborIndex] = currentIndex;
                }
            }
        }
    }

    // build the path by working backwards
    int endIndex = -1;
    for (int i = 0; i < numBuildings; i++) {
        if (allBuildings[i] == end) {
            endIndex = i;
            break;
        }
    }

    if (distances[endIndex] < 0) {
        cout << "No path exists between these buildings.\n";
        return path;
    }

    int step = endIndex;
    while (step != startIndex) {
        path.push_back(allBuildings[step]);
        step = previous[step];
    }
    path.push_back(start);

    // reverse the path so it goes from start to end
    vector<string> finalPath;
    for (int i = path.size() - 1; i >= 0; i--) {
        finalPath.push_back(path[i]);
    }

    return finalPath;
}