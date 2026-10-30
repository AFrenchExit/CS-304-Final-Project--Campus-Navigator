// Brandon and Sahand 
// server.cpp
// connects the dijkstra to the html map and gets the quickest paths from OSRM (osrm would not pull sidewalks only roads).
// most of the code provided from the leaflet and OSRM webistes.

#include "httplib.h"
#include "json.hpp"
#include "graph.h"
#include "dijkstra.h"
#include <iostream>

using namespace std;
using json = nlohmann::json;

int main() {
    Graph campus;
    if (!campus.loadFromJson("../data/buildings.json", "../data/edges.json")) {
        return 1;
    }
 // OSRM and leaflet provided code down below
    httplib::Server svr;

    svr.Options(R"(.*)", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    });

    svr.Get("/find-path", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        string start = req.get_param_value("start");
        string end = req.get_param_value("end");

        cout << "Finding path: " << start << " -> " << end << "\n";

        // run dijkstra to get the list of buildings
        vector<string> path = findShortestPath(campus, start, end);

        if (path.empty()) {
            res.set_content("{\"error\":\"No path\"}", "application/json");
            return;
        }

        // draw real sidewalks we need the GPS coordinates of the start and end buildings
        ifstream bFile("../data/buildings.json");
        json buildings = json::parse(bFile);
        
        double startLat, startLon, endLat, endLon;
        for (auto& b : buildings) {
            if (b["name"] == start) { startLat = b["lat"]; startLon = b["lon"]; }
            if (b["name"] == end) { endLat = b["lat"]; endLon = b["lon"]; }
        }

        // ask OSRM for the actual sidewalk route between those coordinates
        string osrmUrl = "/route/v1/foot/" + to_string(startLon) + "," + to_string(startLat) + ";" + to_string(endLon) + "," + to_string(endLat) + "?overview=full&geometries=geojson";
        
        httplib::Client osrm("https://router.project-osrm.org");
        auto osrmRes = osrm.Get(osrmUrl.c_str());

        if (osrmRes && osrmRes->status == 200) {
            // send the real sidewalk geometry back to the map!
            res.set_content(osrmRes->body, "application/json");
        } else {
            res.set_content("{\"error\":\"OSRM failed\"}", "application/json");
        }
    });

    cout << "Server running on port 8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}