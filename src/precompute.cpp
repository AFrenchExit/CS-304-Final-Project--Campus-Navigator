// *** most of this was given from OSRM website******

// Brandon Tornow and Sahand Shaghaghi
// 10/14/26
// precompute.cpp
// Reads the data from buildings.json and calls OSRM to find the walking
// distances between all buildings and then writes the data/edges.json

#include "httplib.h"   // for talking to OSRM over http
#include "json.hpp"    // for reading and writing json 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
using json = nlohmann::json;  

// OSRM only lets us send 100 coords per request so we split into chunks
// got help from gfg for math stuff: (https://www.geeksforgeeks.org/ceil-floor-functions-cpp/)
const int CHUNK_SIZE = 80;

int main() {
    // open buildings.json 
    ifstream inFile("../data/buildings.json");
    json allB = json::parse(inFile);
    int total = allB.size();
    cout << "loaded " << total << " buildings\n";

    // key ends up looking like: BuildingA|BuildingB.
    map<string, double> distTable;
    int numChunks = (total + CHUNK_SIZE - 1) / CHUNK_SIZE;

    for (int c = 0; c < numChunks; c++) {
        int start = c * CHUNK_SIZE;
        int end = min(start + CHUNK_SIZE, total);
        int len = end - start;

        // OSRM format: lon1,lat1;lon2,lat2;...
        string coordStr = "";
        for (int k = start; k < end; k++) {
            if (k > start) coordStr += ";";
            coordStr += to_string(allB[k]["lon"].get<double>());
            coordStr += ",";
            coordStr += to_string(allB[k]["lat"].get<double>());
        }

        // *** all of this was provided from OSRM website******
        // the /table/v1/foot/ path returns a matrix of distances
        string api = "/table/v1/foot/" + coordStr + "?annotations=distance";
        
        httplib::Client web("https://router.project-osrm.org");
        web.set_read_timeout(180, 0);

        auto resp = web.Get(api.c_str());
        json reply = json::parse(resp->body);

        auto distMat = reply["distances"];

        for (int a = 0; a < len; a++) {
            for (int b = a + 1; b < len; b++) {
                if (!distMat[a][b].is_null()) {
                    double m = distMat[a][b].get<double>();
                    if (m > 0) {
                        string nA = allB[start + a]["name"];
                        string nB = allB[start + b]["name"];
                        if (nA > nB) swap(nA, nB);
                        distTable[nA + "|" + nB] = m;
                    }
                }
            }
        }
    }

    // now that we have all distances, turn the map into a json array
    json edges = json::array();

    // loops through the map. kv.first is the keyn and kv.second is the value for the 'it' we parsed
    for (auto& kv : distTable) {
        size_t pos = kv.first.find('|');
        string n1 = kv.first.substr(0, pos);
        string n2 = kv.first.substr(pos + 1);

        json e;
        e["from"] = n1;
        e["to"]   = n2;
        e["distance_m"] = kv.second;
        edges.push_back(e);
    }

    // write edges.json
    ofstream out("../data/edges.json");
    out << edges.dump(2);
    out.close();

    cout << "wrote " << edges.size() << " edges\n";
    return 0;
}