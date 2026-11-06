// Brandon and Sahand 
// server.cpp
// connects the dijkstra to the html map and gets the quickest paths from OSRM (osrm would not pull sidewalks only roads).
// most of the code provided from the leaflet and OSRM webistes. also got help from gfg for the http server setup (https://www.geeksforgeeks.org/how-to-create-a-simple-http-server-in-cpp/)

#include "httplib.h"
#include "json.hpp"
#include "graph.h"
#include "dijkstra.h"
#include <iostream>
#include <fstream>

using namespace std;
using json = nlohmann::json;

int main() {
    Graph g;
    g.loadFromJson("../data/buildings.json", "../data/edges.json");
    
    // OSRM, leaflet, and geeks for geeks provided code down below
    // svr is our web server object
    httplib::Server svr;

    // send the list of buildings to the html dropdown req is the request from the browser and res is what we send back (used in gfg)
    svr.Get("/buildings", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        ifstream bFile("../data/buildings.json");
        json bldgs = json::parse(bFile);
        
        // .dump() turns the json object back into a string to send to the browser (used in gfg)
        res.set_content(bldgs.dump(), "application/json");
    });

    svr.Get("/find-path", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        string start = req.get_param_value("start");
        string end = req.get_param_value("end");

        // run dijkstra to get the list of buildings
        vector<string> path = findShortestPath(g, start, end);

        // draw real sidewalks we need the GPS coordinates of the start and end buildings
        ifstream bFile("../data/buildings.json");
        json buildings = json::parse(bFile);
        
        // for the log and lad
        double sLat = 0, sLon = 0, eLat = 0, eLon = 0;
        for (auto& b : buildings) {
            if (b["name"] == start) { sLat = b["lat"]; sLon = b["lon"]; }
            if (b["name"] == end) { eLat = b["lat"]; eLon = b["lon"]; }
        }

        // ask OSRM for the actual sidewalk route between those coordinates
        string osrmUrl = "/route/v1/foot/" + to_string(sLon) + "," + to_string(sLat) + ";" + to_string(eLon) + "," + to_string(eLat) + "?overview=full&geometries=geojson&alternatives=true";
        httplib::Client osrm("https://router.project-osrm.org");
        
        auto osrmRes = osrm.Get(osrmUrl.c_str());

        // send the OSRM reply directly to the browser
        res.set_content(osrmRes->body, "application/json");
    });

    cout << "Server running on port 8080\n";
    
    // .listen() starts the server and keeps it running
    svr.listen("0.0.0.0", 8080);
    return 0;
}