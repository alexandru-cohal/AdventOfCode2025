#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include <map>
using namespace std;

int SolvePart1(unsigned int LightDiagramValue, unsigned int NodeMaxValue, vector<unsigned int> LightMaskList)
{
    /**
     * Consider a graph with nodes with values from 0 to NodeMaxValue.
     * Graph edges: between each node and its XORed value with one of the values from LightMaskList.
     * Apply BFS with Start node = 0 and Target node = LightDiagramValue.
     * The length of the shortest path - 1 is the returned value.
     */

    vector<unsigned int> Distance(NodeMaxValue + 1, 0);
    queue<unsigned int> BfsQueue;

    Distance[0] = 1;
    BfsQueue.push(0);

    while (BfsQueue.size() > 0)
    {
        unsigned int NodeCurrent = BfsQueue.front();
        BfsQueue.pop();

        for (int IdxLightMask = 0; IdxLightMask < LightMaskList.size(); IdxLightMask++)
        {
            unsigned int NodeConnected = NodeCurrent ^ LightMaskList[IdxLightMask];

            if (Distance[NodeConnected] == 0)
            {
                Distance[NodeConnected] = Distance[NodeCurrent] + 1;
                if (NodeConnected != LightDiagramValue)
                {
                    BfsQueue.push(NodeConnected);
                }
                else
                {
                    return (Distance[LightDiagramValue] - 1);
                }
            }
        }
    }
    
    return 0;
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
        LightMaskList.clear();
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

void ReadAndPreparePart2(string InputFilename, string OutputMatFilename)
{
    /**
     * Idea: For each machine configuration solve the corresponding underdetermined system of linear equations.
     * The system will be solved in Octave using the glpk function.
     * The input file will be parsed and an .m file for solving directly all systems in Octave will be generated.
     */

    ifstream InputFile(InputFilename);
    ofstream OutputFile(OutputMatFilename);
    string ConfigurationLine, ButtonWiringsString, JoltageRequirementsString;
    map<int, vector<int>> AMatrixHelper;
    vector<int> BVector;

    /**
     * Print the instruction for:
     *   - cleaning the console and the memory
     *   - initializing the total sum of obtained values for the minized objective function
     */
    OutputFile << "clear; clc;" << endl;
    OutputFile << "fminTotal = 0;" << endl;
    OutputFile << endl;

    // Read the input file line by line (i.e. machine configuration by machine configuration)
    while (getline(InputFile, ConfigurationLine))
    {
        OutputFile << "# " << ConfigurationLine << endl;

        // Split the 3 parts of the line: lights diagram, button wiring schematics, joltage requirments
        int PosLightDiagamEnd = ConfigurationLine.find(']') - 1;
        int PosJoltageStart = ConfigurationLine.find('{') + 1;
        int PosJoltageEnd = ConfigurationLine.length() - 2;
        int PosButtonWiringStart = PosLightDiagamEnd + 2;
        int PosButtonWiringEnd = PosJoltageStart - 3;
        
        /**
         * Parse the button wiring schematics and create:
         *   - Firstly the helper for the A matrix (for each counter, create a list of button indices which are increasing that counter)
         *   - Secontly the A matrix itself (a matrix for each an element from line i and column j is 1 if the counter i is increased by button j and 0 otherwise)
         *  */ 
        ButtonWiringsString = ConfigurationLine.substr(PosButtonWiringStart, PosButtonWiringEnd - PosButtonWiringStart + 1);

        // Helper for A matrix
        AMatrixHelper.clear();
        stringstream ButtonWiringsStream(ButtonWiringsString);
        string CurrentButtonWiringString;
        int ButtonIdx = -1;
        while (getline(ButtonWiringsStream, CurrentButtonWiringString, ')'))
        {
            ButtonIdx += 1;

            CurrentButtonWiringString = CurrentButtonWiringString.substr(2, CurrentButtonWiringString.length() - 2);
            stringstream CurrentButtonWiringStream(CurrentButtonWiringString);
            string CounterIdx;
            unsigned int LightMask = 0;
            while (getline(CurrentButtonWiringStream, CounterIdx, ','))
            {
                AMatrixHelper[stoi(CounterIdx)].push_back(ButtonIdx);
            }
        }
        int NumButtons = ButtonIdx + 1;

        // Parse the list of joltage requirements and create the b vector
        JoltageRequirementsString = ConfigurationLine.substr(PosJoltageStart, PosJoltageEnd - PosJoltageStart + 1);

        BVector.clear();
        stringstream JoltageRequirementsStream(JoltageRequirementsString);
        string CurrentJoltageString;
        while (getline(JoltageRequirementsStream, CurrentJoltageString, ','))
        {
            BVector.push_back(stoi(CurrentJoltageString));
        }
        int NumCounters = BVector.size();

        // A matrix
        int AMatrix[NumCounters][NumButtons];
        for (int IdxCounter = 0; IdxCounter < NumCounters; IdxCounter++)
        {
            // Clear the line first
            for (int IdxButton = 0; IdxButton < NumButtons + 1; IdxButton++)
            {
                AMatrix[IdxCounter][IdxButton] = 0;
            }

            // Set to 1 the needed values
            for (int IdxIdxButton = 0; IdxIdxButton < AMatrixHelper[IdxCounter].size(); IdxIdxButton++)
            {
                AMatrix[IdxCounter][AMatrixHelper[IdxCounter][IdxIdxButton]] = 1;
            }
        }

        // Print A matrix
        OutputFile << "A = [";
        for (int IdxCounter = 0; IdxCounter < NumCounters; IdxCounter++)
        {
            if (IdxCounter > 0)
            {
                OutputFile << "; ";
            }
            for (int IdxButton = 0; IdxButton < NumButtons; IdxButton++)
            {
                if (IdxButton > 0)
                {
                    OutputFile << ", ";
                }
                OutputFile << AMatrix[IdxCounter][IdxButton];
            }
        }
        OutputFile << "];" << endl;

        // Print B vector
        OutputFile << "b = [";
        for (int IdxCounter = 0; IdxCounter < NumCounters; IdxCounter++)
        {
            if (IdxCounter > 0)
            {
                OutputFile << ", ";
            }
            OutputFile << BVector[IdxCounter];
        }
        OutputFile << "];" << endl;

        // Print c vector (c = ones(size(A, 2), 1)) 
        OutputFile << "c = [";
        for (int IdxButton = 0; IdxButton < NumButtons; IdxButton++)
        {
            if (IdxButton > 0)
            {
                OutputFile << "; ";
            }
            OutputFile << "1";
        }
        OutputFile << "];" << endl;

        // Print ctype vector (ctype = repmat("S", size(A, 1), 1), where "S" is the contraint of type equality)
        OutputFile << "ctype = [";
        for (int IdxCounter = 0; IdxCounter < NumCounters; IdxCounter++)
        {
            if (IdxCounter > 0)
            {
                OutputFile << "; ";
            }
            OutputFile << "\"S\"";
        }
        OutputFile << "];" << endl;

        // Print vartype vector (vartype = repmat("I", size(A, 2), 1), where "I" is the integer type of the unknown) 
        OutputFile << "vartype = [";
        for (int IdxButton = 0; IdxButton < NumButtons; IdxButton++)
        {
            if (IdxButton > 0)
            {
                OutputFile << "; ";
            }
            OutputFile << "\"I\"";
        }
        OutputFile << "];" << endl;

        // Print the call to the function glpk for solving the system of equations
        OutputFile << "[~, fmin, ~] = glpk(c, A, b, [], [], ctype, vartype);" << endl;

        // Print the instruction for updating the total sum of obtained values for the minized objective function
        OutputFile << "fminTotal += fmin;" << endl;
    }

    // Print the instruction for outputting the result on the console
    OutputFile << endl;
    OutputFile << "disp(fminTotal)" << endl;
    
    InputFile.close();
    OutputFile.close();

    cout << ".m file generated for Part 2" << endl;
}

int main()
{
    cout << ReadAndSolvePart1("input/day10.txt") << endl;

    ReadAndPreparePart2("input/day10.txt", "day10.m");

    return 0;
}