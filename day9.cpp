#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

vector<pair<int, int>> ReadFileLineByLine(string Filename)
{
    ifstream InputFile(Filename);
    string Line, Col;
    vector<pair<int, int>> RedTiles;
    pair<int, int> Coordinates;

    while (getline(InputFile, Line, ','))
    {
        getline(InputFile, Col);
        Coordinates.first = stoi(Line);
        Coordinates.second = stoi(Col);
        RedTiles.push_back(Coordinates);
    }

    return RedTiles;
}

long long SolvePart1(vector<pair<int, int>> RedTiles)
{
    long long MaxArea = 0;

    for (int IdxRedTile1 = 0; IdxRedTile1 < RedTiles.size() - 1; IdxRedTile1++)
    {
        for (int IdxRedTile2 = IdxRedTile1 + 1; IdxRedTile2 < RedTiles.size(); IdxRedTile2++)
        {
            int DiffLine = abs(RedTiles[IdxRedTile1].first - RedTiles[IdxRedTile2].first) + 1;
            int DiffCol = abs(RedTiles[IdxRedTile1].second - RedTiles[IdxRedTile2].second) + 1;
            long long Area = (long long)DiffLine * DiffCol;
            if (Area > MaxArea)
            {
                MaxArea = Area;
            }
        }
    }

    return MaxArea;
}

int main()
{
    vector<pair<int, int>> RedTiles;
    RedTiles = ReadFileLineByLine("input/day9.txt");

    cout << SolvePart1(RedTiles) << endl;

    return 0;
}