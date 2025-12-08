#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int ReadAndSolvePart1(string FileName)
{
    ifstream File(FileName);
    string Line;
    int NumCols;
    int NumSplits = 0;

    // Read the first line
    getline(File, Line);
    NumCols = Line.length();
    vector<bool> BeamOrNot(NumCols, 0);
    for (int IdxCol = 0; IdxCol < NumCols; IdxCol++)
    {
        if (Line[IdxCol] == 'S')
        {
            BeamOrNot[IdxCol] = true;
            break;
        }
    }   

    // Read the following lines and process them one by one
    while (getline(File, Line))
    {
        for (int IdxCol = 0; IdxCol < NumCols; IdxCol++)
        {
            if (BeamOrNot[IdxCol] == true)
            {
                switch (Line[IdxCol])
                {
                    case '.':
                        // Do nothing, the beam continues in the same direction
                        break;
                    case '^':
                        // Split the beam
                        NumSplits++;
                        BeamOrNot[IdxCol - 1]   = true;
                        BeamOrNot[IdxCol]       = false;
                        BeamOrNot[IdxCol + 1]   = true;
                }
            }
        }        
    }

    return NumSplits;
}

long long ReadAndSolvePart2(string FileName)
{
    ifstream File(FileName);
    string Line;
    int NumCols;
    long long TotalNumTimelines = 0;

    // Read the first line
    getline(File, Line);
    NumCols = Line.length();
    vector<long long> NumTimelines(NumCols, 0);
    for (int IdxCol = 0; IdxCol < NumCols; IdxCol++)
    {
        if (Line[IdxCol] == 'S')
        {
            NumTimelines[IdxCol] = 1;
            break;
        }
    }   

    // Read the following lines and process them one by one
    while (getline(File, Line))
    {
        for (int IdxCol = 0; IdxCol < NumCols; IdxCol++)
        {
            if (NumTimelines[IdxCol] != 0)
            {
                switch (Line[IdxCol])
                {
                    case '.':
                        // Do nothing, the beam continues in the same direction
                        break;
                    case '^':
                        // Split the beam
                        NumTimelines[IdxCol - 1] += NumTimelines[IdxCol];
                        NumTimelines[IdxCol + 1] += NumTimelines[IdxCol];
                        NumTimelines[IdxCol] = 0;
                        break;
                }
            }
        }        
    } 

    // Add up the number of timelines obtained in the end for each position
    for (int IdxCol = 0; IdxCol < NumCols; IdxCol++)
    {
        TotalNumTimelines += NumTimelines[IdxCol];
    }

    return TotalNumTimelines;
}

int main()
{
    cout << ReadAndSolvePart1("input/day7.txt") << endl;
    cout << ReadAndSolvePart2("input/day7.txt") << endl;

    return 0;
}