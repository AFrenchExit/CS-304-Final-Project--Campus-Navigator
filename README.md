UTK Campus Navigator

Description: A campus navigation tool for UTK. Finds the shortest walking path between campus buildings using dijkstras algorithm on a weighted graph.

Team Members: Brandon T. Sahand S.

Status: Project is very close to being finished just needs bug fixes and tweaks. It uses a C backend to run the dijkstra and an OSRM api to get the actual walking distances paths. The frontend is a map using leaflet. 

sources we used to get the data for the map:
Building coordinates: Overpass Turbo
Walking distances: OSRM Foot Profile

Folders used:
data: has buildings.json and edges.json
src: all the C++ files graph, dijkstra, server, and precompute
web: just has theindex.html for the map

How to run the:
You must need to open two terminals to run this. One for the dijkstra alg. and one to run the web sever.

Terminal 1 (will need to be in src folder):
g++ -std=c++11 -O2 -DCPPHTTPLIB_OPENSSL_SUPPORT server.cpp graph.cpp dijkstra.cpp -o server -lssl -lcrypto -lpthread
./server

Terminal 2 (need to be in the web folder):
python3 -m http.server 8000

Open the website on port 8000 in your browser. Should have an option to open if ran with VS code 

Notes: 
The ssl flags (-lssl -lcrypto) are required for the https osrm api.