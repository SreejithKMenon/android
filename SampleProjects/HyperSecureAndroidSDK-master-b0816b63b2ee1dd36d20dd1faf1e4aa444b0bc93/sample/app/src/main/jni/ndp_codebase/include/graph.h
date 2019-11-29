/* Copyright (C) HyperVerge, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <list>
#include <vector>

// Graph class represents a undirected graph
// using adjacency list representation
class GraphConnected
{
    private:
        int V;    // No. of vertices

        // Pointer to an array containing adjacency lists
        std::list<int> *adj;

        // A function used by DFS
        void DFSUtil(int v, bool visited[], std::vector<std::vector<int> > &connected);
    public:
        GraphConnected(int V);   // Constructor
        void addEdge(int v, int w);
        std::vector<std::vector<int> > connectedComponents();
};
#endif
