#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

map<string, vector<string>> Graph;
map<string, long long> PathCount;
map<string, bool> NodeVisited;

void ReadFile(string Filename)
{
    ifstream FileInput(Filename);
    string Line;

    while (getline(FileInput, Line))
    {
        stringstream LineStream(Line);
        string NodeSrc, NodeDest;
        vector<string> NodeDestVector;
        getline(LineStream, NodeSrc, ':');
        getline(LineStream, NodeDest, ' '); // Discard the semicolon and the space after NodeSrc
        while (getline(LineStream, NodeDest, ' '))
        {
            NodeDestVector.push_back(NodeDest);
        }
        Graph[NodeSrc] = NodeDestVector;
        PathCount[NodeSrc] = -1;
        NodeVisited[NodeSrc] = false;
    }
}

void CleanHelperVariables()
{
    // Reset the values of all the keys from the PathCount map
    for (auto ItPathCount = PathCount.begin(); ItPathCount != PathCount.end(); ++ItPathCount) 
        ItPathCount->second = -1;

    // Reset the values of all the keys from the NodeVisited map
    for (auto ItNodeVisited = NodeVisited.begin(); ItNodeVisited != NodeVisited.end(); ++ItNodeVisited) 
        ItNodeVisited->second = false;
}

/**
 * Graph Traversal: 
 *  - Backtracking DFS 
 *  - Storing visited nodes (for avoiding cycles) 
 *  - Memoization of calculated paths (for avoiding recomputing the same values)
 * */ 
long long TraverseGraph(string NodeSrc, string NodeDest)
{
    // Corner cases
    if (NodeSrc == NodeDest)
    {
        // The base case when source node is the same as destination node.
        // Then there is only one possibility.
        return 1;
    }
    if (NodeVisited[NodeSrc] == true)
    {
        // The node was already visited which means that a cycle was detected.
        // Then ignore this path in order to avoid endless loops in the cycle.
        return 0;
    }
    if (NodeSrc == "out")
    {
        // Node "out" is not the source of any connection.
        // Then for sure there is no possibility to get from "out" to any other node.
        return 0;
    }
    if (PathCount[NodeSrc] != -1)
    {
        // The node was not already visited but the number of paths was already calculated.
        // So use the already calculated value. 
        return PathCount[NodeSrc];
    }
    
    // No corner case was detected => We are in the basic situation.
    // Use the source node as step on the overall path.
    // Mark the source node as visited, go through all its connected nodes and call the traversing function, 
    // sum up all the returned number of paths, unmark the source node as visited.
    NodeVisited[NodeSrc] = true;
    long long PathCountNodeSrc = 0;
    for(int IdxConn = 0; IdxConn < Graph[NodeSrc].size(); IdxConn++)
    {
        string NodeConn = Graph[NodeSrc][IdxConn];
        PathCountNodeSrc += TraverseGraph(NodeConn, NodeDest);
    }
    NodeVisited[NodeSrc] = false;
    PathCount[NodeSrc] = PathCountNodeSrc;

    return PathCountNodeSrc;
}

int main()
{
    ReadFile("input/day11.txt");

    // Solve Part 1
    cout << TraverseGraph("you", "out") << endl;

    // Solve Part 2
    CleanHelperVariables();
    long long PathCountSvrFft = TraverseGraph("svr", "fft");
    CleanHelperVariables();
    long long PathCountFftDac = TraverseGraph("fft", "dac");
    CleanHelperVariables();
    long long PathCountDacOut = TraverseGraph("dac", "out");
    long long PathCountSvrFftDacOut = PathCountSvrFft * PathCountFftDac * PathCountDacOut;

    CleanHelperVariables();
    long long PathCountSvrDac = TraverseGraph("svr", "dac");
    CleanHelperVariables();
    long long PathCountDacFft = TraverseGraph("dac", "fft");
    CleanHelperVariables();
    long long PathCountFftOut = TraverseGraph("fft", "out");
    long long PathCountSvrDacFftOut = PathCountSvrDac * PathCountDacFft * PathCountFftOut;

    cout << PathCountSvrFftDacOut + PathCountSvrDacFftOut << endl;

    return 0;
}