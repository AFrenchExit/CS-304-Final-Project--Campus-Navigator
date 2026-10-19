// Brandon Tornow and Sahand Shaghaghi
// 10/14/26
// precompute.cpp
// Reads the data from buildings.json and calls OSRM to find the walking
// distances between all buildings and then writes the data/edges.json


// run this from inside src/ folder it looks for ../data/

#include "httplib.h"   // for talking to OSRM over http
#include "json.hpp"    // for reading and writing json 
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
using json = nlohmann::json;  

// OSRM only lets us send 100 coords per request
// 135 buildings is too many, so we split into chunks
const int CHUNK_SIZE = 80;

int main() {

    // open buildings.json 
    ifstream inFile("../data/buildings.json");
    if (!inFile.is_open()) {
        cout << "cant open ../data/buildings.json\n";
        return 1;
    }

    // go through whole file into a json object
    json allBuilds = json::parse(inFile);
    int total = allBuilds.size();
    cout << "loaded " << total << " buildings\n";

    // this map stores every distance we find
    // key looks like "BuildingA|BuildingB" (alphabetical so order doesn't matter)
    // value is meters. using a map means duplicates get deduped automatically
    map<string, double> distTable;

    // how many chunks do we need? 135 / 80 = 2 chunks
    int numChunks = (total + CHUNK_SIZE - 1) / CHUNK_SIZE;

    // loop through each chunk
    for (int chunkIdx = 0; chunkIdx < numChunks; chunkIdx++) {

        int startIdx = chunkIdx * CHUNK_SIZE;
        int endIdx = min(startIdx + CHUNK_SIZE, total);
        int chunkLen = endIdx - startIdx;

        cout << "chunk " << (chunkIdx + 1) << "/" << numChunks
             << " (buildings " << startIdx << " to " << (endIdx - 1) << ")\n";

        // OSRM format: lon1,lat1;lon2,lat2;lon3,lat3;...
        // only build the string for THIS chunk
        string coordStr = "";
        for (int k = startIdx; k < endIdx; k++) {
            if (k > startIdx) coordStr += ";";  // semicolon between points
            coordStr += to_string(allBuilds[k]["lon"].get<double>());
            coordStr += ",";
            coordStr += to_string(allBuilds[k]["lat"].get<double>());
        }

        // *** all of this was provided from OSRM website******

        // call OSRM    
        // /table/v1/foot/ means "walking mode", not driving
        // annotations=distance means give me distances in meters
        string apiPath = "/table/v1/foot/" + coordStr + "?annotations=distance";

        // the "https://" in the constructor tells httplib to use SSL
        httplib::Client web("https://router.project-osrm.org");

        // OSRM can be slow, give it 3 min before giving up
        // (180, 0) means 180 seconds, 0 microseconds
        web.set_read_timeout(180, 0);

        cout << "  calling osrm...\n";
        auto resp = web.Get(apiPath.c_str());

        // resp is a pointer, so check it exists AND the HTTP code is 200 OK
        if (!resp) {
            cout << "  no response for this chunk\n";
            continue;
        }

        if (resp->status != 200) {
            cout << "  http status: " << resp->status << "\n";
            cout << "  body: " << resp->body.substr(0, 200) << "\n";
            continue;
        }

        json osrmReply = json::parse(resp->body);

        // OSRM always includes a "code" field. "Ok" means success
        if (osrmReply["code"] != "Ok") {
            cout << "  osrm said: " << osrmReply["code"] << "\n";
            continue;
        }

        // "distances" is a 2D array. meters from bldg i to bldg j
        auto distMat = osrmReply["distances"];

        // turn matrix into edges. only store i < j so we don't double count
        for (int a = 0; a < chunkLen; a++) {
            for (int b = a + 1; b < chunkLen; b++) {

                // matrix cells can be null if OSRM couldn't find a route
                if (!distMat[a][b].is_null()) {
                    double meters = distMat[a][b].get<double>();

                    // ignore 0 distance pairs (same building)
                    if (meters > 0) {
                        string nameA = allBuilds[startIdx + a]["name"];
                        string nameB = allBuilds[startIdx + b]["name"];

                        // sort alphabetically so the key is consistent
                        // so "A|B" and "B|A" both become "A|B"
                        if (nameA > nameB) swap(nameA, nameB);
                        string key = nameA + "|" + nameB;

                        distTable[key] = meters;
                    }
                }
            }
        }

        cout << "  done, " << distTable.size() << " total pairs so far\n";
    }

    // now that we have all distances, turn the map into a json array
    json edgeList = json::array();

    for (auto& kv : distTable) {
        // split the key back into two names at the "|" character
        size_t barPos = kv.first.find('|');
        string n1 = kv.first.substr(0, barPos);
        string n2 = kv.first.substr(barPos + 1);

        json oneEdge;
        oneEdge["from"] = n1;
        oneEdge["to"]   = n2;
        oneEdge["distance_m"] = kv.second;
        edgeList.push_back(oneEdge);
    }

    // write edges.json
    // dump(2) = pretty-print with 2-space indent so it's readable
    ofstream outFile("../data/edges.json");
    outFile << edgeList.dump(2);
    outFile.close();

    cout << "wrote " << edgeList.size() << " edges to ../data/edges.json\n";
    return 0;
}
