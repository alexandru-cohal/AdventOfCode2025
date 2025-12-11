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

typedef enum
{
    DIR_HORIZ = 0,
    DIR_VERT_DESC,
    DIR_VERT_ASC
} DirectionType;

typedef struct
{
    pair<int, int> Point1;
    pair<int, int> Point2;
    DirectionType Direction;
} SegmentType;

bool InsidePolygon(int PointLine, int PointCol, vector<SegmentType> Segments)
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

    for (int IdxSegment = 0; IdxSegment < Segments.size(); IdxSegment++)
    {
        if (Segments[IdxSegment].Direction == DIR_VERT_ASC || Segments[IdxSegment].Direction == DIR_VERT_DESC)
        {
            // Vertical Edge
            int EdgeLineMin = min(Segments[IdxSegment].Point1.first, Segments[IdxSegment].Point2.first);
            int EdgeLineMax = max(Segments[IdxSegment].Point1.first, Segments[IdxSegment].Point2.first);

            if ((PointCol == Segments[IdxSegment].Point1.second) && (EdgeLineMin <= PointLine) && (PointLine <= EdgeLineMax))
            {
                // Exactly on the edge
                OnEdge = true;
                break;
            }
            else
            {
                // Not on the edge. Check if the ray intersects it
                if (PointCol < Segments[IdxSegment].Point1.second)
                {
                    if ((EdgeLineMin < PointLine) && (PointLine < EdgeLineMax))
                    {
                        // Intersection in the inner part of the edge
                        NumIntersections++;
                    }
                    else if ((EdgeLineMin == PointLine) || (PointLine == EdgeLineMax))
                    {
                        // Intersection in one of the ends of the edge
                        int IdxSegmentNext = (IdxSegment + 2) % Segments.size();
                        if ((Segments[IdxSegment].Direction == DIR_VERT_DESC && Segments[IdxSegmentNext].Direction == DIR_VERT_DESC) ||
                            (Segments[IdxSegment].Direction == DIR_VERT_ASC && Segments[IdxSegmentNext].Direction == DIR_VERT_ASC))
                        {
                            NumIntersections++;
                        }
                    }
                }
            }
        }
        else
        {
            // Horizontal edge
            int EdgeColMin = min(Segments[IdxSegment].Point1.second, Segments[IdxSegment].Point2.second);
            int EdgeColMax = max(Segments[IdxSegment].Point1.second, Segments[IdxSegment].Point2.second);

            if ((PointLine == Segments[IdxSegment].Point1.first) && (EdgeColMin <= PointCol) && (PointCol <= EdgeColMax))
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

    vector<SegmentType> Segments;
    RedTiles.push_back(RedTiles[0]);
    for (int IdxRedTile = 0; IdxRedTile < RedTiles.size(); IdxRedTile++)
    {
        SegmentType Segment;
        Segment.Point1 = RedTiles[IdxRedTile];
        Segment.Point2 = RedTiles[IdxRedTile + 1];
        if (RedTiles[IdxRedTile].first == RedTiles[IdxRedTile + 1].first)
        {
            Segment.Direction = DIR_HORIZ;
        }
        else
        {
            if (RedTiles[IdxRedTile].second < RedTiles[IdxRedTile + 1].second)
            {
                Segment.Direction = DIR_VERT_ASC;
            }
            else
            {
                Segment.Direction = DIR_VERT_DESC;
            }

        }
        Segments.push_back(Segment);
    }
    
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

            if (InsidePolygon(RectangleLineMin, RectangleColMin, Segments) == true &&
                InsidePolygon(RectangleLineMin, RectangleColMax, Segments) == true &&
                InsidePolygon(RectangleLineMax, RectangleColMin, Segments) == true &&
                InsidePolygon(RectangleLineMax, RectangleColMax, Segments) == true)
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