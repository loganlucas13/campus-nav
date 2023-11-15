// testing.cpp <Starter Code>
// <Your name>
//
// Adam T Koehler, PhD
// University of Illinois Chicago
// CS 251, Fall 2023
//
// Project Original Variartion By:
// Joe Hummel, PhD
// University of Illinois at Chicago
//
// This file is used for testing graph.h.  We encourage you to use Google's
// test framework for this project, but it is not required (because we will
// not be grading the tests file).
//

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <fstream>

#include "graph.h"

using namespace std;


//
// buildGraph:
//
// Inputs the graph vertices and edges from the given file, building
// the graph g.  File format:
//   vertex
//   vertex
//   ...
//   #
//   src dest weight
//   src dest weight
//   ...
//   #
//
void buildGraph(string filename, graph<string,int>& G)
{
  ifstream file(filename);
  string   v;

  if (!file.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << filename << "'." << endl;
    cout << endl;
    return;
  }

  //
  // Input vertices as single uppercase letters:  A B C ... #
  //
  file >> v;

  while (v != "#")
  {
    if (!G.addVertex(v))
      cout << "**Error: unable to add vertex '" << v << "', why not?" << endl;

    file >> v;
  }

  //
  // Now input edges:  Src Dest Weight ... #
  //
  string src, dest;
  int  weight;

  file >> src;

  while (src != "#")
  {
    file >> dest;
    file >> weight;

    if (!G.addEdge(src, dest, weight))
      cout << "**Error: unable to add edge (" << src << "," << dest << "," << weight << "), why not?" << endl;

    file >> src;
  }
}

//
// outputGraph:
//
// Outputs graph g to the console.
//
void outputGraph(graph<string,int>& G)
{
  vector<string> vertices = G.getVertices();

  cout << "**Vertices: ";

  for (string v : vertices)
  {
    cout << v << " ";
  }

  cout << endl;

  cout << "**Edges: ";

  for (string v : vertices)
  {
    set<string> neighbors = G.neighbors(v);

    for (string n : neighbors)
    {
      int weight;

      if (G.getWeight(v, n, weight))
      {
        cout << "(" << v << "," << n << "," << weight << ") ";
      }
      else
      {
        cout << "(" << v << "," << n << "," << "???" << ") ";
      }
    }
  }

  cout << endl;
}


int main()
{
	graph<string, int> G1;
	graph<string, int> G2;
	string filename;
	string filename2;
	string startV;

	filename = "graph.txt";
	filename2 = "graph2.txt";


	cout << "graph 1:\n";
	buildGraph(filename, G1);
	outputGraph(G1);
  cout << "\n";
	G1.dump(cout);


	cout << "\n\nclearing graph 1!\n";
  cout << "graph 1:\n";
	G1.clear();
	outputGraph(G1);


	cout << "\n\nrebuilding graph 1!\n";
  cout << "graph 1:\n";
	buildGraph(filename2, G1);
	outputGraph(G1);
  cout << "\n";
	G1.dump(cout);


	cout << "\n\ntesting = operator!\n";
	cout << "graph 2:\n";
	G2 = G1;
	outputGraph(G2);
  cout << "\n";
	G2.dump(cout);

	return 0;
}
