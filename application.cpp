// application.cpp

// Logan Lucas
// CS 251
// Fall 2023
// Project 5: Open Street Maps

// This program finds the nearest destination route throughout nodes using Dijkstra's algorithm and graphs


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
#include <queue>
#include <stack>

#include "tinyxml2.h"
#include "dist.h"
#include "graph.h"
#include "osm.h"


using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::max();

typedef pair<long long, double> vertexDistance;
typedef pair<map<long long, double>, map<long long, long long>> distancesAndPredecessors;

class prioritize {
	public:
	bool operator()(const vertexDistance& p1, const vertexDistance& p2) const {
		return p1.second > p2.second;
	}
};


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
BuildingInfo findNearestBuilding(const Coordinates c1, const vector<BuildingInfo> buildings, set<string> unreachableBuildings) {
	double min = INF;

	BuildingInfo nearestBuilding;
	double distance = 0.0;
	for (auto building : buildings) {
		Coordinates c2 = building.Coords;
		distance = distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);
		if (distance < min && unreachableBuildings.find(building.Fullname) == unreachableBuildings.end()) {
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


// @brief: prints out a building's info (formatted)
// @param: building - building to print
// @return: none
void printBuilding(const BuildingInfo building) {
	cout << " " << building.Fullname << "\n";
	cout << " (" << building.Coords.Lat << ", " << building.Coords.Lon << ")\n";
}


// @brief: prints out a node's info (formatted)
// @param: node - node to print
// @return: none
void printNode(const Coordinates node) {
	cout << " " << node.ID << "\n";
	cout << " (" << node.Lat << ", " << node.Lon << ")\n";
}


// @brief: run's Dijkstra's algorithm
// @param: start - first vertex's ID
// @param: G - graph of all nodes
// @return: none
distancesAndPredecessors dijkstras(long long start, graph<long long, double> G) {
	priority_queue<vertexDistance, vector<vertexDistance>, prioritize> unvisitedQueue;

	map<long long, double> distances;
	map<long long, long long> predecessors;

	set<long long> visited;

	for (auto vertex : G.getVertices()) {
		distances.emplace(vertex, INF);
		predecessors.emplace(vertex, 0);
		vertexDistance newDistance;
		if (vertex != start) {
			newDistance = make_pair(vertex, INF);
		}
		else {
			distances[start] = 0;
			newDistance = make_pair(start, 0);
		}
		unvisitedQueue.push(newDistance);
	}

	while (!unvisitedQueue.empty()) {
		long long currentVertex = unvisitedQueue.top().first;
		unvisitedQueue.pop();

		if (distances[currentVertex] == INF) {
			break;
		}
		else if (visited.find(currentVertex) != visited.end()) {
			continue;
		}
		else {
			visited.emplace(currentVertex);

			for (auto adjacentVertex : G.neighbors(currentVertex)) {
				double edgeWeight;
				G.getWeight(currentVertex, adjacentVertex, edgeWeight); // updates edgeWeight by reference

				double altDistance = distances[currentVertex] + edgeWeight;

				if (altDistance < distances[adjacentVertex]) {
					distances[adjacentVertex] = altDistance;
					predecessors[adjacentVertex] = currentVertex;
				}
				vertexDistance newPair = make_pair(adjacentVertex, altDistance);
				unvisitedQueue.push(newPair);
			}
		}
	}
	distancesAndPredecessors returnVal = make_pair(distances, predecessors);
	return returnVal;
}


// @brief: gets the path from a vertex to its destination
// @param: endVertex: final vertex in a path
// @param: predecessors: predecessors of each vertex
// @return: path (vector)
vector<long long> getPath(long long endVertex, map<long long, long long> predecessors) {
	vector<long long> path;

	stack<long long> stack;

	long long currentVertex = endVertex;
	while (currentVertex) {
		stack.push(currentVertex);
		currentVertex = predecessors[currentVertex];
	}

	while (stack.size() != 0) {
		path.push_back(stack.top());
		stack.pop();
	}

	return path;
}


// @brief: prints the path (formatted)
// @param: path - path to print
// @return: none
void printPath(vector<long long> path) {
	cout << "Path: ";
	for (size_t i = 0; i < path.size(); i++) {
		cout << path[i];
		if (i+1 != path.size()) {
			cout << "->";
		}
	}
	cout << "\n";
}


// @brief: standard application implementation
// @param: Nodes - map with node ID's and coordinates
// @param: Footways - vector of all footways
// @param: Buildings - vector of all buildings
// @param: G - graph implementation
// @return: none
void application(
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
	vector<BuildingInfo>& Buildings, graph<long long, double>& G) {
	string person1Building, person2Building;

	set<string> unreachableBuildings;

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

			BuildingInfo buildingCenter = findNearestBuilding(midpoint, Buildings, unreachableBuildings);

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

			// runs Dijkstra's algorithm

			map<long long, double> distances1;
			map<long long, long long> predecessors1;

			map<long long, double> distances2;
			map<long long, long long> predecessors2;

			distancesAndPredecessors dijkstraReturn1 = dijkstras(node1.ID, G);
			distancesAndPredecessors dijkstraReturn2 = dijkstras(node2.ID, G);

			distances1 = dijkstraReturn1.first;
			predecessors1 = dijkstraReturn1.second;

			distances2 = dijkstraReturn2.first;
			predecessors2 = dijkstraReturn2.second;

			if (distances1[node2.ID] >= INF) { // enter new buildings
				cout << "Sorry, destination unreachable.\n";
			}
			else if (distances1[nodeCenter.ID] >= INF || distances2[nodeCenter.ID] >= INF) {
				cout << "At least one person was unable to reach the destination building. Finding next closest building...\n";
				unreachableBuildings.emplace(buildingCenter.Fullname);
				// loops again (new buildings from input)
			}
			else {
				double distance1 = distances1[nodeCenter.ID];
				vector<long long> path1 = getPath(nodeCenter.ID, predecessors1);

				double distance2 = distances2[nodeCenter.ID];
				vector<long long> path2 = getPath(nodeCenter.ID, predecessors2);

				cout << "Person 1's distance to dest: " << distance1 << " miles\n";
				printPath(path1);

				cout << "\n";

				cout << "Person 2's distance to dest: " << distance2 << " miles\n";
				printPath(path2);
			}
		}
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
