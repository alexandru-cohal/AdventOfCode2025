#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<string> ReadFileLineByLine(string FileName)
{
    vector<string> FileLines;
    ifstream File(FileName);
    string Line;

    while (getline(File, Line))
    {
        FileLines.push_back(Line);
    }
    File.close();

    return FileLines;
}

int SolvePart1(vector<string> InputLines)
{
    int TotalValue = 0;

    for (int IndexInputLine = 0; IndexInputLine < InputLines.size(); IndexInputLine++)
    {
        char MaxValue = '0' - 1;
        int MaxIndex = 0;

        for (int IndexElement = 0; IndexElement < InputLines[IndexInputLine].length() - 1; IndexElement++)
        {
            if (InputLines[IndexInputLine][IndexElement] > MaxValue)
            {
                MaxValue = InputLines[IndexInputLine][IndexElement];
                MaxIndex = IndexElement; 
            }
        }
        
        TotalValue += (MaxValue - '0') * 10;

        MaxValue = '0' - 1;
        for (int IndexElement = MaxIndex + 1; IndexElement < InputLines[IndexInputLine].length(); IndexElement++)
        {
            if (InputLines[IndexInputLine][IndexElement] > MaxValue)
            {
                MaxValue = InputLines[IndexInputLine][IndexElement];
            }
        }

        TotalValue += (MaxValue - '0');
    }

    return TotalValue;
}

pair<int, int> FindMaxValueAndIndex(string Line, int LeftIndexLimit, int RightIndexLimit)
{
    int MaxValue = '0' - 1;
    int MaxIndex = 0;

    for (int IndexElement = LeftIndexLimit; IndexElement <= RightIndexLimit; IndexElement++)
    {
        if (Line[IndexElement] > MaxValue)
        {
            MaxValue = Line[IndexElement];
            MaxIndex = IndexElement; 
        }
    }

    return {MaxValue - '0', MaxIndex};
}

long long SolvePart2(vector<string> InputLines)
{
    long long TotalValue = 0;
    long long CurrentValue;
    pair<int, int> MaxValueAndIndex;
    int LeftIndexLimit, RightIndexLimit;

    for (int IndexInputLine = 0; IndexInputLine < InputLines.size(); IndexInputLine++)
    {
        CurrentValue = 0;
        MaxValueAndIndex = {0, -1};

        for (int ValueCount = 0; ValueCount < 12; ValueCount++)
        {
            LeftIndexLimit = MaxValueAndIndex.second + 1;
            RightIndexLimit = InputLines[IndexInputLine].length() - (12 - ValueCount);
            
            MaxValueAndIndex = FindMaxValueAndIndex(InputLines[IndexInputLine], LeftIndexLimit, RightIndexLimit);
            CurrentValue = (CurrentValue * 10) + MaxValueAndIndex.first;
        }

        TotalValue += CurrentValue;
    }

    return TotalValue;
}

int main()
{
    vector<string> InputLines;

    InputLines = ReadFileLineByLine("input/day3.txt");

    cout << SolvePart1(InputLines) << endl;
    cout << SolvePart2(InputLines) << endl;
    
    return 0;
}