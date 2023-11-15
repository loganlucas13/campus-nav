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
        struct Edge {
            VertexT end;
            WeightT weight;
        };

        map<VertexT, vector<Edge> adjList;

    public:
        // @brief: default constructor for graph class
        graph() {

        }


        // @brief: removes all vertices and edges
        void clear() {

        }


        // @brief: copy all vertices and edges into a new graph
        // @param: other - graph to copy from (const)
        // @return: new graph
        graph& operator=(const graph &other) {
            return this;
        }


        // @brief: gets total # of vertices in graph
        // @return: # of vertices in graph
        int NumVertices() const {
            return -1;
        }


        // @brief: gets total # of edges in graph
        // @return: # of edges in graph
        int NumEdges() const {
            return -1;
        }


        // @brief: adds a new vertex to the graph (no edges)
        // @param: v - vertex to add
        // @return: false if vertex already exists, true otherwise
        bool addVertex(VertexT v) {
            return false;           
        }


        // @brief: adds a weighted edge from vertex *from* to vertex *to*
        //         overwrites weight value if edge already exists in vertex *to*
        // @param: from - beginning vertex
        // @param: to - destination vertex
        // @param: weight - weight value of edge
        // @return: false if either vertex doesn't exist, otherwise true
        bool addEdge(VertexT from, VertexT to, WeightT weight) {
            return false;
        }


        // @brief: gets the weight of the edge from vertex *from* to vertex *to*
        // @param: from - beginning vertex
        // @param: to - destination vertex
        // @param: weight - pass by reference; updated during function call
        // @return: false if either vertex doesn't exist, otherwise true
        bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
            return false;
        }


        // @brief: returns a set of all neighbors of a given vertex
        //         "neighbors" == all vertices that the given vertex has an edge to
        // @param: v - starting vertex
        // @return: set of all neighbors of v
        set<VertexT> neighbors(VertexT v ) const {
            return {};
        }


        // @brief: returns all vertices in the graph
        // @return: vector of all vertices
        vector<VertexT> getVertices() const {

        }

        // @brief: for debugging purposes
        void dump(ostream& output) {

        }
};
