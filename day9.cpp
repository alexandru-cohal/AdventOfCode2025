#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>
using namespace std;

const int STEP_LINE[4] = {0, -1, 0, 1};
const int STEP_COL[4] = {-1, 0, 1, 0}; 

vector<pair<int, int>> ReadFileLineByLine(string Filename)
{
    ifstream InputFile(Filename);
    string Line, Col;
    vector<pair<int, int>> RedTiles;
    pair<int, int> Coordinates;

    while (getline(InputFile, Line, ','))
    {
        getline(InputFile, Col);
        Coordinates.second = stoi(Line);
        Coordinates.first = stoi(Col);
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

bool InsidePolygon(int PointLine, int PointCol, vector<pair<int, int>> PointsPolygon)
{
    int NumIntersections = 0;
    bool OnEdge = false;

    /**
     * Ray casting algorithm:
     * Consider a horizontal ray (line = PointLine). 
     * Check only the vertical edges of the polygon.
     * Check only the vertical edges on a column to the right of the PointCol.
     * If the ray intersects the limit of an edge => Consider it.
     */

    for (int IdxPoint = 0; IdxPoint < PointsPolygon.size() - 1; IdxPoint++)
    {
        if (PointsPolygon[IdxPoint].second == PointsPolygon[IdxPoint + 1].second)
        {
            // Vertical Edge
            int EdgeLineMin = min(PointsPolygon[IdxPoint].first, PointsPolygon[IdxPoint + 1].first);
            int EdgeLineMax = max(PointsPolygon[IdxPoint].first, PointsPolygon[IdxPoint + 1].first);

            if ((PointCol == PointsPolygon[IdxPoint].second) && (EdgeLineMin <= PointLine) && (PointLine <= EdgeLineMax))
            {
                // Exactly on the edge
                OnEdge = true;
                break;
            }
            else
            {
                // Not on the edge. Check if the ray intersects it
                if (PointCol < PointsPolygon[IdxPoint].second)
                {
                    if ((EdgeLineMin <= PointLine) && (PointLine <= EdgeLineMax))
                    {
                        NumIntersections++;
                    }
                }
            }
        }
        else
        {
            // Horizontal edge
            int EdgeColMin = min(PointsPolygon[IdxPoint].second, PointsPolygon[IdxPoint + 1].second);
            int EdgeColMax = max(PointsPolygon[IdxPoint].second, PointsPolygon[IdxPoint + 1].second);

            if ((PointLine == PointsPolygon[IdxPoint].first) && (EdgeColMin <= PointCol) && (PointCol <= EdgeColMax))
            {
                // Exactly on the edge
                OnEdge = true;
                break;
            }
        }
    }

    if ((OnEdge == true) || (NumIntersections % 2 == 1))
    {
        return true;
    }

    return false;
}

long long SolvePart2(vector<pair<int, int>> RedTiles)
{
    long long MaxArea = 0;

    RedTiles.push_back(RedTiles[0]);
    
    for (int IdxRedTile1 = 0; IdxRedTile1 < RedTiles.size() - 2; IdxRedTile1++)
    {
        for (int IdxRedTile2 = IdxRedTile1 + 1; IdxRedTile2 < RedTiles.size() - 1; IdxRedTile2++)
        {
            int RectangleLineMax = max(RedTiles[IdxRedTile1].first, RedTiles[IdxRedTile2].first);
            int RectangleLineMin = min(RedTiles[IdxRedTile1].first, RedTiles[IdxRedTile2].first);
            int RectangleColMax = max(RedTiles[IdxRedTile1].second, RedTiles[IdxRedTile2].second);
            int RectangleColMin = min(RedTiles[IdxRedTile1].second, RedTiles[IdxRedTile2].second);

            if (IdxRedTile1 == 65 && IdxRedTile2 == 309)
            {
                int stoppoint = 1;
            }

            if (InsidePolygon(RectangleLineMin, RectangleColMin, RedTiles) == true &&
                InsidePolygon(RectangleLineMin, RectangleColMax, RedTiles) == true &&
                InsidePolygon(RectangleLineMax, RectangleColMin, RedTiles) == true &&
                InsidePolygon(RectangleLineMax, RectangleColMax, RedTiles) == true)
            {
                int DiffRectangleLine = RectangleLineMax - RectangleLineMin + 1;
                int DiffRectangleCol = RectangleColMax - RectangleColMin + 1;
                long long AreaRectangle = (long long)DiffRectangleLine * DiffRectangleCol;

                if (AreaRectangle > MaxArea)
                {
                    MaxArea = AreaRectangle;
                }
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
    cout << SolvePart2(RedTiles) << endl;

    return 0;
}