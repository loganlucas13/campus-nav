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

		// TODO: MILESTONE 7: search buildings 1 and 2

		// while (path is not found) {
		// TODO: MILESTONE 8: locate center building

		// TODO: MILESTONE 9: find nearest nodes from buildings 1, 2, and center

		// TODO: MILESTONE 10: run Dijkstra's algorithm

		// TODO: MILESTONE 11: find second nearest destination (loop again)

		// }

		// cout << "Person 1's building not found" << endl;
		// cout << "Person 2's building not found" << endl;


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

	// TODO: MILESTONE 5: add vertices - DONE
	for (auto node : Nodes) {
		G.addVertex(node.first);
	}

	// TODO: MILESTONE 6: add edges



	// TODO: uncomment code after milestone 6

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
