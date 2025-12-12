#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Store the sizes of the presents without reading them from the input file
vector<int> PresentSizes = {7, 7, 7, 6, 5, 7};

int ReadFileAndSolvePart1(string Filename)
{
    ifstream InputFile(Filename);
    string Line;
    int NumLines = 0;

    while (getline(InputFile, Line))
    {
        NumLines++;
        stringstream LineStringStream(Line);

        long long Area = 1;
        string Dimension;
        getline(LineStringStream, Dimension, 'x');
        Area *= stoi(Dimension);
        getline(LineStringStream, Dimension, ':');
        Area *= stoi(Dimension);

        int IdxCurrentType = -1;
        long long OccupiedArea = 0;
        string TypeCount;
        getline(LineStringStream, TypeCount, ' ');
        while (getline(LineStringStream, TypeCount, ' '))
        {
            IdxCurrentType++;
            OccupiedArea += PresentSizes[IdxCurrentType] * stoi(TypeCount);
        }
        if (OccupiedArea > Area)
        {
            NumLines--;
        }
    }

    return NumLines;
}

int main()
{
    cout << ReadFileAndSolvePart1("input/day12.txt") << endl;

    return 0;
}