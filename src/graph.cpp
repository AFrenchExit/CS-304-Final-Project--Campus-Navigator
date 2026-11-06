// Brandon and Sahand
// graph.cpp
// this is the implemntation of the graph class from graph.h

#include "graph.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// bPath is buildings.json, ePath is edges.json
bool Graph::loadFromJson(string bPath, string ePath) {
    
    // load buildings first
    ifstream bIn(bPath.c_str());
    
    // json parse turns the file into a json object we can loop through
    // got help from gfg for parsing json: (https://www.geeksforgeeks.org/cpp-program-to-read-and-print-a-json-file/)
    json bData = json::parse(bIn);

    // loop through every building in the json and add it as an empty list
    for (int i = 0; i < bData.size(); i++) {
        string name = bData[i]["name"];
        adjList[name] = vector<Edge>(); 
    }

    // load the edges next
    ifstream eIn(ePath.c_str());
    json eData = json::parse(eIn);

    for (int i = 0; i < eData.size(); i++) {
        string from = eData[i]["from"];
        string to = eData[i]["to"];
        double dist = eData[i]["distance_m"];

        // since walking is undirected add it both ways
        Edge e1;
        e1.destination = to;
        e1.distance = dist;
        adjList[from].push_back(e1);

        Edge e2;
        e2.destination = from;
        e2.distance = dist;
        adjList[to].push_back(e2);
    }

    cout << "Graph loaded: " << adjList.size() << " buildings.\n";
    return true;
}

vector<Edge> Graph::getNeighbors(string b) {
    // is a quick way to check if the building exists in our map. if it does we return its list of edges
    if (adjList.count(b) > 0) {
        return adjList[b];
    }
    // return empty vec if not found
    vector<Edge> empty;
    return empty;
}

// this checks if the building name exists in our graph
bool Graph::hasBuilding(string b) {
    return adjList.count(b) > 0;
}

vector<string> Graph::getAllBuildings() {
    vector<string> names;
    
    // an iterator loop. 'it' points to key and value
    // got help from gfg for map iterators: (https://www.geeksforgeeks.org/map-iterator-in-cpp-stl/)
    for (unordered_map<string, vector<Edge>>::iterator it = adjList.begin(); it != adjList.end(); ++it) {
        names.push_back(it->first);
    }
    return names;
}