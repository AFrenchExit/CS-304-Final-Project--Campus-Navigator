UTK Campus Navigator

Description: A campus navigation tool for UTK. Finds the shortest walking path between campus buildings using Dijkstra's algorithm on a weighted graph.

Team Members: Brandon T. Sahand S.

Status: Data collection done. Got 135 building coordinates from OpenStreetMap and found the walking distances from OSRM. Pushed edges.json and the precompute tool to the repo. Next is too writing the graph and Dijkstra code then the Leaflet map for the frontend.

# Data sources:

got building coordinates from Overpass Turbo 

got walking distances from OSRM website


# How to compile and run the precompute tool:
cd src

g++ -std=c++11 -O2 -DCPPHTTPLIB_OPENSSL_SUPPORT -o precompute precompute.cpp -lssl -lcrypto -lpthread

./precompute

# Previw the output: 
* This only show the first 30 lines

head -30 ../data/edges.json

wc -l ../data/edges.json
