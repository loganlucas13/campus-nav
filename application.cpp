// application.cpp <Starter Code>
// Logan Lucas
//
//
// Adam T Koehler, PhD
// University of Illinois Chicago
// CS 251, Fall 2023
//
// Project Original Variartion By:
// Joe Hummel, PhD
// University of Illinois at Chicago
//
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "tinyxml2.h"
#include "dist.h"
#include "graph.h"
#include "osm.h"


using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::max();


// @brief: searches for building in Buildings vector
// @param: result - used as a return
// @param: query - search query
// @param: buildings - vector of ALL buildings
// @return: true if found, false otherwise
bool searchBuilding(BuildingInfo& result, const string query, const vector<BuildingInfo> buildings) {
	// searches for abbreviation
	for (auto building : buildings) {
		if (building.Abbrev == query) {
			result = building;
			return true;
		}
	}

	// searches for partial name
	for (auto building : buildings) {
		if (building.Fullname.find(query) != string::npos) {
			result = building;
			return true;
		}
	}

	return false; // abbreviation AND partial match not found
}


// @brief: finds the nearest building from a set of coordinates
// @param: c1 - starting coordinates
// @param: buildings - vector of ALL buildings
// @return: nearest building
BuildingInfo findNearestBuilding(const Coordinates c1, const vector<BuildingInfo> buildings) {
	double min = INF;

	BuildingInfo nearestBuilding;
	double distance = 0.0;
	for (auto building : buildings) {
		Coordinates c2 = building.Coords;
		distance = distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);
		if (distance < min) {
			nearestBuilding = building;
			min = distance;
		}
	}
	return nearestBuilding;
}


// @brief: finds the nearest node from a building
// @param: building - starting building
// @param: nodes - map of ALL nodes
// @param: footways - vector of ALL footways
// @return: nearest node
Coordinates findNearestNode(const BuildingInfo building, const map<long long, Coordinates> nodes, const vector<FootwayInfo> footways) {
	double min = INF;

	Coordinates c1 = building.Coords;

	Coordinates nearestNode;
	double distance = 0.0;
	for (auto footway : footways) {
		for (auto node : footway.Nodes) { // loops through all nodes in each footway
			Coordinates c2 = nodes.at(node); // finds footway's node in nodes map
			distance = distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);
			if (distance < min) {
				nearestNode = c2;
				min = distance;
			}
		}
	}
	return nearestNode;
}


void printBuilding(const BuildingInfo building) {
	cout << " " << building.Fullname << "\n";
	cout << " (" << building.Coords.Lat << ", " << building.Coords.Lon << ")\n";
}


void printNode(const Coordinates node) {
	cout << " " << node.ID << "\n";
	cout << " (" << node.Lat << ", " << node.Lon << ")\n";
}

//
// Implement your standard application here
//
void application(
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
	vector<BuildingInfo>& Buildings, graph<long long, double>& G) {
	string person1Building, person2Building;

	cout << endl;
	cout << "Enter person 1's building (partial name or abbreviation), or #> ";
	getline(cin, person1Building);
	while (person1Building != "#") {
		cout << "Enter person 2's building (partial name or abbreviation)> ";
		getline(cin, person2Building);

		// searches for buildings 1 and 2
		BuildingInfo building1;
		BuildingInfo building2;

		if (!searchBuilding(building1, person1Building, Buildings)) {
			cout << "Person 1's building not found\n";
		}
		else if (!searchBuilding(building2, person2Building, Buildings)) {
			cout << "Person 2's building not found\n";
		}
		else {
			
			// locates center building
			Coordinates c1 = building1.Coords;
			Coordinates c2 = building2.Coords;

			Coordinates midpoint = centerBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);

			BuildingInfo buildingCenter = findNearestBuilding(midpoint, Buildings);

			cout << "Person 1's point:\n";
			printBuilding(building1);

			cout << "Person 2's point:\n";
			printBuilding(building2);

			cout << "Destination Building:\n";
			printBuilding(buildingCenter);
			cout << "\n";

			// finds nearest nodes from each building (building1, building2, and the center/destination building)
			Coordinates node1 = findNearestNode(building1, Nodes, Footways);
			Coordinates node2 = findNearestNode(building2, Nodes, Footways);
			Coordinates nodeCenter = findNearestNode(buildingCenter, Nodes, Footways);

			cout << "Nearest P1 node:\n";
			printNode(node1);

			cout << "Nearest P2 node:\n";
			printNode(node2);

			cout << "Nearest destination node:\n";
			printNode(nodeCenter);
			cout << "\n";

			// TODO: MILESTONE 10: run Dijkstra's algorithm

			// TODO: MILESTONE 11: find second nearest destination (loop again)
		}
		//
		// another navigation?
		//
		cout << endl;
		cout << "Enter person 1's building (partial name or abbreviation), or #> ";
		getline(cin, person1Building);
	}
}

int main() {
	graph<long long, double> G;

	// maps a Node ID to it's coordinates (lat, lon)
	map<long long, Coordinates>  Nodes;
	// info about each footway, in no particular order
	vector<FootwayInfo>          Footways;
	// info about each building, in no particular order
	vector<BuildingInfo>         Buildings;
	XMLDocument                  xmldoc;

	cout << "** Navigating UIC open street map **" << endl;
	cout << endl;
	cout << std::setprecision(8);

	string def_filename = "map.osm";
	string filename;

	cout << "Enter map filename> ";
	getline(cin, filename);

	if (filename == "") {
		filename = def_filename;
	}

	//
	// Load XML-based map file
	//
	if (!LoadOpenStreetMap(filename, xmldoc)) {
		cout << "**Error: unable to load open street map." << endl;
		cout << endl;
		return 0;
	}

	//
	// Read the nodes, which are the various known positions on the map:
	//
	int nodeCount = ReadMapNodes(xmldoc, Nodes);

	//
	// Read the footways, which are the walking paths:
	//
	int footwayCount = ReadFootways(xmldoc, Footways);

	//
	// Read the university buildings:
	//
	int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

	//
	// Stats
	//
	assert(nodeCount == (int)Nodes.size());
	assert(footwayCount == (int)Footways.size());
	assert(buildingCount == (int)Buildings.size());

	cout << endl;
	cout << "# of nodes: " << Nodes.size() << endl;
	cout << "# of footways: " << Footways.size() << endl;
	cout << "# of buildings: " << Buildings.size() << endl;

	// add vertices
	for (auto node : Nodes) {
		G.addVertex(node.first);
	}
	
	// add edges
	for (auto footway : Footways) {
		for (size_t i = 0; i < footway.Nodes.size()-1; i++) {
			Coordinates c1 = Nodes[footway.Nodes[i]];
			Coordinates c2 = Nodes[footway.Nodes[i+1]];

			double distance = distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);

			G.addEdge(footway.Nodes[i], footway.Nodes[i+1], distance);
			G.addEdge(footway.Nodes[i+1], footway.Nodes[i], distance);
		}
	}

	cout << "# of vertices: " << G.NumVertices() << endl;
	cout << "# of edges: " << G.NumEdges() << endl;
	cout << endl;

	// Execute Application
	application(Nodes, Footways, Buildings, G);

	//
	// done:
	//
	cout << "** Done **" << endl;
	return 0;
}
