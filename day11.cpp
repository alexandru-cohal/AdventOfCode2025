#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

map<string, vector<string>> Graph;
map<string, int> PathCount;
int SolutionPart2 = 0;

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
        PathCount[NodeSrc] = 0;
    }
}

int SolvePart1(string NodeSrc, string NodeDest)
{
    queue<string> BfsQueue;
    BfsQueue.push(NodeSrc);
    PathCount[NodeSrc] = 1;

    while (BfsQueue.size() > 0)
    {
        string NodeCurr = BfsQueue.front();
        BfsQueue.pop();

        for(int IdxConn = 0; IdxConn < Graph[NodeCurr].size(); IdxConn++)
        {
            string NodeConn = Graph[NodeCurr][IdxConn];

            PathCount[NodeConn]++;
            BfsQueue.push(NodeConn);
        }
    }

    return PathCount[NodeDest];
}

int main()
{
    ReadFile("input/day11.txt");

    cout << SolvePart1("you", "out") << endl;

    return 0;
}