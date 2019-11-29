/* Copyright (C) HyperVerge, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <stdio.h>
#include <iostream>
#include "graph.h"

// Method to print connected components in an
// undirected graph
std::vector<std::vector<int> > GraphConnected::connectedComponents()
{
    // Mark all the vertices as not visited
    std::vector<std::vector<int> > connected;
    connected.push_back(std::vector<int>());
    bool *visited = new bool[V];
    for(int v = 0; v < V; v++)
        visited[v] = false;
 
    for (int v=0; v<V; v++)
    {
        if (visited[v] == false)
        {
            // print all reachable vertices
            // from v
            DFSUtil(v, visited, connected);
            connected.push_back(std::vector<int>()); 
        }
    }
    delete visited;
    return connected;
}
 
void GraphConnected::DFSUtil(int v, bool visited[], std::vector<std::vector<int> > &connected)
{
    // Mark the current node as visited and print it
    visited[v] = true;
    connected[connected.size() - 1].push_back(v);
 
    // Recur for all the vertices
    // adjacent to this vertex
    std::list<int>::iterator i;
    for(i = adj[v].begin(); i != adj[v].end(); ++i)
        if(!visited[*i])
            DFSUtil(*i, visited, connected);
}
 
GraphConnected::GraphConnected(int V)
{
    this->V = V;
    adj = new std::list<int>[V];
}
 
// method to add an undirected edge
void GraphConnected::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

