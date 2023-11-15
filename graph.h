// graph.h <Starter Code>
// < Your name >
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
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

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
    private:
		typedef pair<VertexT, WeightT> Edge;
        map<VertexT, set<Edge>> adjList;

		int numEdges;

    public:
        // @brief: default constructor for graph class
        graph() {
			this->numEdges = 0;
        }


        // @brief: removes all vertices and edges
        void clear() {
			for (auto vertexPair : this->adjList) {
				vertexPair.second.clear(); // clears edge set
			}
			this->adjList.clear(); // clears vertices

			this->numEdges = 0;
        }


        // @brief: copy all vertices and edges into a new graph
        // @param: other - graph to copy from (const)
        // @return: new graph
        graph& operator=(const graph &other) {
			this->clear();

			set<Edge> edges;
            for (auto vertexPair : other.adjList) {
				edges.clear(); // erases previous vertex's edges
				for (auto edgePair : vertexPair.second) { // creates edge set
					Edge newEdge = make_pair(edgePair.first, edgePair.second);
					edges.emplace(newEdge);
				}
				this->adjList.emplace(vertexPair.first, edges);
			}
			edges.clear();

			this->numEdges = other.numEdges;

			return *this;
        }


        // @brief: gets total # of vertices in graph
        // @return: # of vertices in graph
        int NumVertices() const {
            return this->adjList.size();
        }


        // @brief: gets total # of edges in graph
        // @return: # of edges in graph
        int NumEdges() const {
            return this->numEdges;
        }


        // @brief: adds a new vertex to the graph (no edges)
        // @param: v - vertex to add
        // @return: false if vertex already exists, true otherwise
        bool addVertex(VertexT v) {
			if (this->adjList.find(v) != this->adjList.end()) { // std::map.find() time complexity: O(log n)
				return false;
			}

			set<Edge> edges = {};
			this->adjList.emplace(v, edges);

			return true;        
        }


        // @brief: adds a weighted edge from vertex *from* to vertex *to*
        //         overwrites weight value if edge already exists in vertex *to*
        // @param: from - beginning vertex
        // @param: to - destination vertex
        // @param: weight - weight value of edge
        // @return: false if either vertex doesn't exist, otherwise true
        bool addEdge(VertexT from, VertexT to, WeightT weight) {
			if (this->adjList.find(from) == this->adjList.end() || // std::map.find() time complexity: O(log n)
				this->adjList.find(to) == this->adjList.end()) {
				return false;
			}

			// add edge in order from->to
			Edge toAdd = make_pair(to, weight);
			
			// time complexity: O(E) where E is the number of edges in a vertex 
			// (permitted in project documentation on page 4)
			for (auto edgePair : adjList[from]) { // checks for duplicate edges
				if (edgePair.first == toAdd.first) {
					adjList[from].erase(edgePair); // since adjList[from] is a std::set, we must erase the element and reinsert w/ a new weight value
					this->numEdges--; // makes sure that this->numEdges doesn't count the ORIGINAL element and the NEW element
					break;
				}
			}

			this->adjList[from].emplace(toAdd);

			this->numEdges++;

            return true;
        }


        // @brief: gets the weight of the edge from vertex *from* to vertex *to*
        // @param: from - beginning vertex
        // @param: to - destination vertex
        // @param: weight - pass by reference; updated during function call
        // @return: false if either vertex doesn't exist, otherwise true
        bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
            if (this->adjList.find(from) == this->adjList.end() || // std::map.find() time complexity: O(log n)
				this->adjList.find(to) == this->adjList.end()) {
				return false;
			}

			// time complexity: O(E) where E is the number of edges in a vertex 
			// (permitted in project documentation on page 4)
			for (auto edgePair : this->adjList.at(from)) {
				if (edgePair.first == to) { // match found!
					weight = edgePair.second;
					break;
				}
			}
			return true;
        }


        // @brief: returns a set of all neighbors of a given vertex
        //         "neighbors" == all vertices that the given vertex has an edge to
        // @param: v - starting vertex
        // @return: set of all neighbors of v
        set<VertexT> neighbors(VertexT v) const {
			set<VertexT> neighbors;

			for (auto edgePair : this->adjList.at(v)) {
				neighbors.emplace(edgePair.first);
			}

            return neighbors;
        }


        // @brief: returns all vertices in the graph
        // @return: vector of all vertices
        vector<VertexT> getVertices() const {
			vector<VertexT> vertices;

			for (auto vertexPair : this->adjList) {
				vertices.push_back(vertexPair.first);
			}

			return vertices;
        }


        // @brief: for debugging purposes
		//         outputs graph information to a stream
		// @param: output - stream being output to
        void dump(ostream& output) {
			output << "***************************************************\n";
			output << "********************* GRAPH ***********************\n";

			output << "**Num vertices: " << this->NumVertices() << endl;
			output << "**Num edges: " << this->NumEdges() << endl;

			for (auto vertexPair : this->adjList) {
				cout << vertexPair.first << ": ";
				if (vertexPair.second.size() == 0) {
					output << "No edges\n";
				}
				else {
					for (auto edgePair : vertexPair.second) {
						output << "(" << edgePair.first << ", " << edgePair.second << ") ";
					}
					output << "\n";
				}
			}
        }
};
