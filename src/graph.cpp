// Brandon and Sahand
// graph.cpp
// this is the implemntation of the graph class from graph.h

#include "graph.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool Graph::loadFromJson(string buildingsFile, string edgesFile) {
    // need to load buildings
    ifstream bFile(buildingsFile.c_str());
    json buildingsData = json::parse(bFile);

    // loop through every building in the json 
    for (int i = 0; i < buildingsData.size(); i++) {
        string name = buildingsData[i]["name"];
        adjList[name] = vector<Edge>(); 
    }

    // load all the edges
    ifstream eFile(edgesFile.c_str());
    if (!eFile.is_open()) {
        cout << "Error: Could not open " << edgesFile << "\n";
        return false;
    }
    json edgesData = json::parse(eFile);

    for (int i = 0; i < edgesData.size(); i++) {
        string from = edgesData[i]["from"];
        string to = edgesData[i]["to"];
        double dist = edgesData[i]["distance_m"];

        // since walking is undirected add it both ways
        Edge edge1;
        edge1.destination = to;
        edge1.distance = dist;
        adjList[from].push_back(edge1);

        Edge edge2;
        edge2.destination = from;
        edge2.distance = dist;
        adjList[to].push_back(edge2);
    }

    cout << "Graph loaded: " << adjList.size() << " buildings.\n";
    return true;
}

vector<Edge> Graph::getNeighbors(string building) {
    // check if the building we added exists in the array
    if (adjList.count(building) > 0) {
        return adjList[building];
    }
    // return empty vec if not found
    vector<Edge> empty;
    return empty;
}

bool Graph::hasBuilding(string building) {
    if (adjList.count(building) > 0) {
        return true;
    }
    return false;
}

vector<string> Graph::getAllBuildings() {
    vector<string> names;
    
    // an iterator loop
    for (unordered_map<string, vector<Edge>>::iterator it = adjList.begin(); it != adjList.end(); ++it) {
        names.push_back(it->first);
    }
    return names;
}