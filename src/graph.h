// Brandon and Sahand
// 10/22/26
// graph.h
// this will help us define the graph class using an adjacency list

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include "json.hpp"

using namespace std;

// represents a single walking path to another building
struct Edge {
    string destination;
    double distance; 
};

class Graph {

public:

    // loads all the buildings and edges from the json files
    bool loadFromJson(string buildingsFile, string edgesFile);

    // returns a list of nearby neighbors for buildings
    vector<Edge> getNeighbors(string building);

    // checks if a building exists in the graph
    bool hasBuilding(string building);

    // returns a list of buldings
    vector<string> getAllBuildings();

private:
    // A map where the key is a building name, and the value is a list of edges
    unordered_map<string, vector<Edge>> adjList;
};

#endif