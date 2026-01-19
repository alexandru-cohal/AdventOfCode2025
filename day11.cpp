#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;

map<string, vector<string>> Graph, GraphReverse;
map<string, long long> NumPath;

void ReadFile(string Filename)
{
    ifstream FileInput(Filename);
    string Line;

    while (getline(FileInput, Line))
    {
        stringstream LineStream(Line);
        string NodeSrc, NodeDest;

        getline(LineStream, NodeSrc, ':');
        Graph[NodeSrc] = {};

        getline(LineStream, NodeDest, ' '); // Discard the semicolon and the space after NodeSrc

        while (getline(LineStream, NodeDest, ' '))
        {
            Graph[NodeSrc].push_back(NodeDest);

            if (GraphReverse.find(NodeDest) == GraphReverse.end())
            {
                GraphReverse[NodeDest] = {};
            }
            GraphReverse[NodeDest].push_back(NodeSrc);

            NumPath[NodeSrc] = -1;
        }
    }
}

void GetReachable(string Node, map<string, vector<string>> Graph, set<string> &NodeReachable)
{
    queue<string> BfsQueue = {};
    BfsQueue.push(Node);

    while (BfsQueue.size())
    {
        string NodeCurr = BfsQueue.front();
        BfsQueue.pop();

        NodeReachable.insert(NodeCurr);

        for (int IdxConn = 0; IdxConn < Graph[NodeCurr].size(); IdxConn++)
        {
            string NodeConn = Graph[NodeCurr][IdxConn];
            if (NodeReachable.find(NodeConn) == NodeReachable.end())
            {
                BfsQueue.push(NodeConn);
            }
        }
    }
}

long long CalculatePath(string NodeCurr, string NodeDest, map<string, vector<string>> Graph, set<string> NodeReachable)
{
    if (NodeCurr == NodeDest)
    {
        return 1;
    }
    else
    {
        if (NumPath[NodeCurr] == -1)
        {
            NumPath[NodeCurr] = 0;
            for (int IdxConn = 0; IdxConn < Graph[NodeCurr].size(); IdxConn++)
            {
                string NodeConn = Graph[NodeCurr][IdxConn];
                if (NodeReachable.find(NodeConn) != NodeReachable.end())
                {
                    NumPath[NodeCurr] += CalculatePath(NodeConn, NodeDest, Graph, NodeReachable);
                }
            }
        }
        return NumPath[NodeCurr];
    }
}

void ResetNumPath()
{
    for (auto NumPathElem : NumPath)
    {
        NumPathElem.second = -1;
    }
}

long long SolvePart2()
{
    set<string> NodeReachableFft = {};
    GetReachable("fft", GraphReverse, NodeReachableFft);
    ResetNumPath();
    long long NumSvr2Fft = CalculatePath("svr", "fft", Graph, NodeReachableFft);
    cout << "svr 2 fft: " << NumSvr2Fft << endl;
    ResetNumPath();
    long long NumDac2Fft = CalculatePath("dac", "fft", Graph, NodeReachableFft);
    cout << "dac 2 fft: " << NumDac2Fft << endl;

    set<string> NodeReachableDac = {};
    GetReachable("dac", GraphReverse, NodeReachableDac);
    ResetNumPath();
    long long NumSvr2Dac = CalculatePath("svr", "dac", Graph, NodeReachableDac);
    cout << "svr 2 dac: " << NumSvr2Dac << endl;
    ResetNumPath();
    long long NumFft2Dac = CalculatePath("fft", "dac", Graph, NodeReachableDac);
    cout << "fft 2 dac: " << NumFft2Dac << endl;

    set<string> NodeReachableOut = {};
    GetReachable("dac", GraphReverse, NodeReachableOut);
    ResetNumPath();
    long long NumFft2Out = CalculatePath("fft", "out", Graph, NodeReachableOut);
    cout << "fft 2 out: " << NumFft2Out << endl;
    ResetNumPath();
    long long NumDac2Out = CalculatePath("dac", "out", Graph, NodeReachableOut);
    cout << "dac 2 out: " << NumDac2Out << endl;

    return (NumSvr2Fft * NumFft2Dac * NumDac2Out) + (NumSvr2Dac * NumDac2Fft * NumFft2Out);
}

int main()
{
    ReadFile("input/day11.txt");

    //cout << SolvePart1() << endl;
    cout << SolvePart2() << endl;

    return 0;
}