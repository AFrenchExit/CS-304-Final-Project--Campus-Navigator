// Brandon and Sahand 
// dijkstra.h
// this is the header for dijkstra

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <string>
#include <vector>

using namespace std;

// finds the shortest path between start and end also returns a list of building names 
vector<string> findShortestPath(Graph& campus, string start, string end);

#endif