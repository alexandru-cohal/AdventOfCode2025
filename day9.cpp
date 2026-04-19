#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>
using namespace std;

vector<pair<int, int>> ReadFileLineByLine(string Filename)
{
    ifstream InputFile(Filename);
    string Line, Col;
    vector<pair<int, int>> RedTiles;
    pair<int, int> Coordinates;

    while (getline(InputFile, Col, ','))
    {
        getline(InputFile, Line);

        Coordinates.first = stoi(Line);
        Coordinates.second = stoi(Col);

        RedTiles.push_back(Coordinates);
    }

    return RedTiles;
}

long long SolvePart1(vector<pair<int, int>> RedTiles)
{
    long long MaxArea = 0;

    // For every combination of 2 red tiles, calculate the area of the rectangle determined by them and find the maximum area
    for (int IdxRedTile1 = 0; IdxRedTile1 < RedTiles.size() - 1; IdxRedTile1++)
    {
        for (int IdxRedTile2 = IdxRedTile1 + 1; IdxRedTile2 < RedTiles.size(); IdxRedTile2++)
        {
            int DiffLine = abs(RedTiles[IdxRedTile1].first - RedTiles[IdxRedTile2].first) + 1;
            int DiffCol = abs(RedTiles[IdxRedTile1].second - RedTiles[IdxRedTile2].second) + 1;
            long long Area = (long long)DiffLine * DiffCol;

            MaxArea = max(MaxArea, Area);
        }
    }

    return MaxArea;
}

bool CheckPointInsidePolygon(int PointLine, int PointCol, vector<pair<int, int>> NodePolygon)
{
    /**
     * Check 1: 
     *      - If the point is on any vertical / horizontal edge => Classify the point as inside the polygon
     *      - If the point is on any vertical / horizontal edge => Move to Check 2
     * Check 2: Apply Ray Casting Algorithm
     *      - Consider a horizontal ray starting from the point to the right side
     *      - Count how many vertical edges this ray intersects
     *          - For avoining double counting, strict check for edge's first point
     *      - If the number of intersections is odd => Classify the point as inside the polygon
     *      - If the number of intersections is even => Classify the point as outside the polygon
     */

    int NumEdgeIntersections = 0;

    // Iterate over the polygon nodes and create the polygon edges as the pair of points (EdgeLine1, EdgeCol1) and (EdgeLine2, EdgeCol2)
    for (int IdxNodePolygon = 0; IdxNodePolygon < NodePolygon.size(); IdxNodePolygon++)
    {
        int EdgeLine1, EdgeCol1, EdgeLine2, EdgeCol2;
        EdgeLine1 = NodePolygon[IdxNodePolygon].first;
        EdgeCol1 = NodePolygon[IdxNodePolygon].second;
        if (IdxNodePolygon < NodePolygon.size() - 1)
        {
            EdgeLine2 = NodePolygon[IdxNodePolygon + 1].first;
            EdgeCol2 = NodePolygon[IdxNodePolygon + 1].second;
        }
        else
        {
            // Consider the edge between the last node from the polygon list and the first one
            EdgeLine2 = NodePolygon[0].first;
            EdgeCol2 = NodePolygon[0].second;
        }

        if (EdgeCol1 == EdgeCol2)
        {
            // Vertical edge

            // Check if the point is exactly on the edge
            if ((PointCol == EdgeCol1) && (min(EdgeLine1, EdgeLine2) <= PointLine) && (PointLine <= max(EdgeLine1, EdgeLine2)))
            {
                // The point is on the edge => Classify the point directly as inside the polygon
                return true;
            }

            // Check if the ray from the point towards the right side intersects the edge (strict check for edge's first point for avoiding double counting)
            if ((PointCol < EdgeCol1) && (min(EdgeLine1, EdgeLine2) < PointLine) && (PointLine <= max(EdgeLine1, EdgeLine2)))
            {
                // The ray from the point towards the right side intersects the edge => Increase the number of intersections
                NumEdgeIntersections++;
            }
        }
        else
        {
            // Horizontal edge

            // Check if the point is exactly on the edge
            if ((PointLine == EdgeLine1) && (min(EdgeCol1, EdgeCol2) <= PointCol) && (PointCol <= max(EdgeCol1, EdgeCol2)))
            {
                // The point is on the edge => Classify the point directly as inside the polygon
                return true;
            }

            // No need to check if the ray from the point towards the right side intersects the edge as this edge will be covered by one of the vertical edges (avoid double counting)
        }
    }

    // If the number of intersections is odd => Classify the point as inside the polygon
    if (NumEdgeIntersections % 2 == 1)
    {
        return true;
    }
    return false;
}

bool CheckSegmentIntersectsPolygonEdges(int Line1, int Col1, int Line2, int Col2, vector<pair<int, int>> NodePolygon)
{
    // Check if the segment is horizontal or vertical
    bool SegmentHorizontal = (Line1 == Line2) ? true : false;

    // Iterate over the polygon nodes and create the polygon edges as the pair of points (EdgeLine1, EdgeCol1) and (EdgeLine2, EdgeCol2)
    for (int IdxNodePolygon = 0; IdxNodePolygon < NodePolygon.size(); IdxNodePolygon++)
    {
        int EdgeLine1, EdgeCol1, EdgeLine2, EdgeCol2;
        EdgeLine1 = NodePolygon[IdxNodePolygon].first;
        EdgeCol1 = NodePolygon[IdxNodePolygon].second;
        if (IdxNodePolygon < NodePolygon.size() - 1)
        {
            EdgeLine2 = NodePolygon[IdxNodePolygon + 1].first;
            EdgeCol2 = NodePolygon[IdxNodePolygon + 1].second;
        }
        else
        {
            // Consider the edge between the last node from the polygon list and the first one
            EdgeLine2 = NodePolygon[0].first;
            EdgeCol2 = NodePolygon[0].second;
        }

        // Check if the edge is horizontal or vertical
        bool EdgeHorizontal = (EdgeLine1 == EdgeLine2) ? true : false;

        // Check the intersection if the segment is horizontal and the edge is vertical
        if ((SegmentHorizontal == true) && (EdgeHorizontal == false))
        {
            if ((min(Col1, Col2) < EdgeCol1) && (EdgeCol1 < max(Col1, Col2)) && 
                (min(EdgeLine1, EdgeLine2) < Line1) && (Line1 < max(EdgeLine1, EdgeLine2)))
            {
                return true;
            }
        }

        // Check the intersection if the segment is vertical and the edge is horizontal
        if ((SegmentHorizontal == false) && (EdgeHorizontal == true))
        {
            if ((min(Line1, Line2) < EdgeLine1) && (EdgeLine1 < max(Line1, Line2)) && 
                (min(EdgeCol1, EdgeCol2) < Col1) && (Col1 < max(EdgeCol1, EdgeCol2)))
            {
                return true;
            }
        }
    }

    return false;
}

long long SolvePart2(vector<pair<int, int>> RedTiles)
{
    long long MaxArea = 0;
    
    // For every combination of 2 red tiles, check if the rectangle determined by them is completely inside the polygon. 
    // If yes, calculate the area of the rectangle and find the maximum area.
    for (int IdxRedTile1 = 0; IdxRedTile1 < RedTiles.size() - 1; IdxRedTile1++)
    {
        for (int IdxRedTile2 = IdxRedTile1 + 1; IdxRedTile2 < RedTiles.size(); IdxRedTile2++)
        {
            int RectangleLineMax = max(RedTiles[IdxRedTile1].first, RedTiles[IdxRedTile2].first);
            int RectangleLineMin = min(RedTiles[IdxRedTile1].first, RedTiles[IdxRedTile2].first);
            int RectangleColMax = max(RedTiles[IdxRedTile1].second, RedTiles[IdxRedTile2].second);
            int RectangleColMin = min(RedTiles[IdxRedTile1].second, RedTiles[IdxRedTile2].second);

            /**
             * The rectangle is inside the polygon (which can be concave) if both conditions are fulfilled:
             *      1. All the nodes of the rectangle are inside the polygon
             *      2. The rectangle's edges are not crossing any polygon's edges
             */ 

            if (CheckPointInsidePolygon(RectangleLineMin, RectangleColMin, RedTiles) == true &&
                CheckPointInsidePolygon(RectangleLineMin, RectangleColMax, RedTiles) == true &&
                CheckPointInsidePolygon(RectangleLineMax, RectangleColMin, RedTiles) == true &&
                CheckPointInsidePolygon(RectangleLineMax, RectangleColMax, RedTiles) == true &&
                CheckSegmentIntersectsPolygonEdges(RectangleLineMin, RectangleColMin, RectangleLineMin, RectangleColMax, RedTiles) == false &&
                CheckSegmentIntersectsPolygonEdges(RectangleLineMin, RectangleColMax, RectangleLineMax, RectangleColMax, RedTiles) == false &&
                CheckSegmentIntersectsPolygonEdges(RectangleLineMax, RectangleColMax, RectangleLineMax, RectangleColMin, RedTiles) == false &&
                CheckSegmentIntersectsPolygonEdges(RectangleLineMax, RectangleColMin, RectangleLineMin, RectangleColMin, RedTiles) == false)
            {
                int DiffLine = RectangleLineMax - RectangleLineMin + 1;
                int DiffCol = RectangleColMax - RectangleColMin + 1;
                long long Area = (long long)DiffLine * DiffCol;

                MaxArea = max(MaxArea, Area);
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