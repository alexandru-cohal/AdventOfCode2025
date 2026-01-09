#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
using namespace std;

int SolvePart1(unsigned int LightDiagramValue, unsigned int NodeMaxValue, vector<unsigned int> LightMaskList)
{
    /**
     * Create a graph with nodes with values from 0 to NodeMaxValue.
     * Graph edges: between each node and its XORed value with one of the values from LightMaskList.
     * Apply BFS with Start node = 0 and Target node = LightDiagramValue.
     * The length of the shortest path - 1 is the returned value.
     */

    vector<set<unsigned int>> Edges;
    for (unsigned int Node = 0; Node <= NodeMaxValue; Node++)
        Edges.push_back({});

    for (unsigned int Node = 0; Node <= NodeMaxValue; Node++)
    {
        for (int IdxLightMask = 0; IdxLightMask < LightMaskList.size(); IdxLightMask++)
        {
            unsigned int NodeConnected = Node ^ LightMaskList[IdxLightMask];

            Edges[Node].insert(NodeConnected);
            Edges[NodeConnected].insert(Node);
        }
    }

    vector<unsigned int> Distance(NodeMaxValue + 1, 0);
    vector<bool> NodeVisited(NodeMaxValue + 1, false);
    queue<unsigned int> BfsQueue;
    BfsQueue.push(0);
    NodeVisited[0] = true;
    while (BfsQueue.size() > 0)
    {
        unsigned int CurrentNode = BfsQueue.front();
        BfsQueue.pop();

        for(unsigned int ConnectedNode : Edges[CurrentNode])
        {
            Distance[ConnectedNode] = Distance[CurrentNode] + 1;
            if (ConnectedNode != LightDiagramValue)
            {
                if (NodeVisited[ConnectedNode] == false)
                {
                    NodeVisited[ConnectedNode] = true;
                    BfsQueue.push(ConnectedNode);
                }
            }
            else
            {
                break;
            }
        }
    }
    
    return Distance[LightDiagramValue];
}

int ReadAndSolvePart1(string Filename)
{
    ifstream InputFile(Filename);
    string ConfigurationLine, LightDiagram, ButtonToggleList, JoltageListString;
    unsigned int LightDiagramValue;
    vector<int> JoltageList;
    vector<unsigned int> LightMaskList;
    int TotalPart1 = 0;

    // Read the input file line by line
    while (getline(InputFile, ConfigurationLine))
    {
        // Split the 3 parts of the line
        int PosLightDiagamStart = 1;
        int PosLightDiagamEnd = ConfigurationLine.find(']') - 1;
        int PosJoltageStart = ConfigurationLine.find('{') + 1;
        int PosJoltageEnd = ConfigurationLine.length() - 2;
        int PosButtonToggleStart = PosLightDiagamEnd + 2;
        int PosButtonToggleEnd = PosJoltageStart - 3;

        LightDiagram = ConfigurationLine.substr(PosLightDiagamStart, PosLightDiagamEnd - PosLightDiagamStart + 1);
        ButtonToggleList = ConfigurationLine.substr(PosButtonToggleStart, PosButtonToggleEnd - PosButtonToggleStart + 1);
        JoltageListString = ConfigurationLine.substr(PosJoltageStart, PosJoltageEnd - PosJoltageStart + 1);

        // Get the light diagram value as unisgned int (# = 1, . = 0), LSB on the right side
        LightDiagramValue = 0;
        for (int IdxLightDiagram = LightDiagram.length() - 1; IdxLightDiagram >= 0; IdxLightDiagram--)
        {
            LightDiagramValue = LightDiagramValue << 1;
            if (LightDiagram[IdxLightDiagram] == '#')
            {
                LightDiagramValue += 1;
            }
        }

        // Get the list of light masks as unsigned int, LSB (light with index 0) on the right side
        stringstream AllLightToggleStream(ButtonToggleList);
        string LightToggleForOneSwitch;
        while (getline(AllLightToggleStream, LightToggleForOneSwitch, ')'))
        {
            LightToggleForOneSwitch = LightToggleForOneSwitch.substr(2, LightToggleForOneSwitch.length() - 2);
            
            stringstream LightToggleForOneSwitchStream(LightToggleForOneSwitch);
            string Light;
            unsigned int LightMask = 0;
            while (getline(LightToggleForOneSwitchStream, Light, ','))
            {
                LightMask += pow(2, stoi(Light));
            }
            LightMaskList.push_back(LightMask);
        }

        // Get the list of joltages
        JoltageList.clear();
        stringstream JoltageListStream(JoltageListString);
        string Joltage;
        while (getline(JoltageListStream, Joltage, ','))
        {
            JoltageList.push_back(stoi(Joltage));
        }

        // Solve for the current parsed line
        TotalPart1 += SolvePart1(LightDiagramValue, pow(2, LightDiagram.length()) - 1, LightMaskList);
    }

    return TotalPart1;
}

int main()
{
    cout << ReadAndSolvePart1("input/day10.txt") << endl;

    return 0;
}