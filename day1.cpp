#include <fstream>
#include <iostream>
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

int SolvePart1(vector<string> FileLines)
{
    int Position = 50;
    int PositionZero = 0;

    for(int IdxLine = 0; IdxLine < FileLines.size(); IdxLine++)
    {
        char Direction = FileLines[IdxLine][0];
        int Steps = stoi(FileLines[IdxLine].erase(0, 1));
        Steps = (Direction == 'L') ? -Steps : Steps;

        Position += Steps;

        while (Position < 0)
        {
            Position += 100;
        }

        while (Position > 99)
        {
            Position -= 100;
        }

        if (Position == 0)
        {
            PositionZero++;
        }
    }
    
    return PositionZero;
}

int SolvePart2(vector<string> FileLines)
{
    int Position = 50;
    int PositionZeroPass = 0;
    bool NegativeLoop, PositiveLoop;

    for(int IdxLine = 0; IdxLine < FileLines.size(); IdxLine++)
    {
        char Direction = FileLines[IdxLine][0];
        int Steps = stoi(FileLines[IdxLine].erase(0, 1));
        Steps = (Direction == 'L') ? -Steps : Steps;

        if ((Position == 0) && (Steps < 0))
        {
            PositionZeroPass--;
        }

        Position += Steps;

        while (Position < 0)
        {
            Position += 100;
            PositionZeroPass++;
        }

        while (Position > 99)
        {
            Position -= 100;
            PositionZeroPass++;

            if (Position == 0)
            {
                PositionZeroPass--;
            }
        }

        if (Position == 0)
        {
            PositionZeroPass++;
        }
    }

    return PositionZeroPass;
}

int main()
{
    vector<string> FileLines;
    FileLines = ReadFileLineByLine("input/day1.txt");

    cout << SolvePart1(FileLines) << endl;
    cout << SolvePart2(FileLines) << endl;

    return 0;
}