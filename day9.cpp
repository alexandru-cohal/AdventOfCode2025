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

long long SolvePart2(vector<pair<int, int>> RedTiles)
{
    // Initialize the floor matrix with 1s
    int MaxLine = 0, MaxCol = 0;
    for (int IdxRedTile = 0; IdxRedTile < RedTiles.size(); IdxRedTile++)
    {
        MaxLine = (RedTiles[IdxRedTile].first > MaxLine) ? RedTiles[IdxRedTile].first : MaxLine;
        MaxCol = (RedTiles[IdxRedTile].second > MaxCol) ? RedTiles[IdxRedTile].second : MaxCol;
    }
    /** 
     * +2 because this will be used as the size of the floor matrix and 
     * +1 is in order to capture the maximum value and 
     * +1 is for having a border where for sure there are no red tiles in order to be able to start the fill algorithm later
    */
    MaxLine += 2; 
    MaxCol += 2;
    vector<vector<int>> Floor(MaxLine, vector<int>(MaxCol, 1));

    // Create the border with 2s
    cout << "---- Bordering ----" << endl;

    RedTiles.push_back(RedTiles[0]);
    for (int IdxRedTile = 0; IdxRedTile < RedTiles.size() - 1; IdxRedTile++)
    {
        pair<int, int> Tile1 = RedTiles[IdxRedTile];
        pair<int, int> Tile2 = RedTiles[IdxRedTile + 1];
        if (Tile1.first == Tile2.first)
        {
            // Horizontal border
            for (int IdxCol = min(Tile1.second, Tile2.second); IdxCol <= max(Tile1.second, Tile2.second); IdxCol++)
            {
                Floor[Tile1.first][IdxCol] = 2;
            }
        }
        else
        {
            // Vertical border
            for (int IdxLine = min(Tile1.first, Tile2.first); IdxLine <= max(Tile1.first, Tile2.first); IdxLine++)
            {
                Floor[IdxLine][Tile1.second] = 2;
            }
        }
    }
    RedTiles.pop_back();

    // Fill the outer part of the shape defined by red tiles with 0s
    cout << "---- Filling ----" << endl;

    pair<int, int> StartTile = {MaxLine - 1, MaxCol - 1};
    Floor[StartTile.first][StartTile.second] = 0;

    queue<pair<int, int>> FillQueue;
    FillQueue.push(StartTile);

    while (FillQueue.size() != 0)
    {
        pair<int, int> CurrentTile = FillQueue.front();
        FillQueue.pop();

        for (int IdxStep = 0; IdxStep < 4; IdxStep++)
        {
            int NewLine = CurrentTile.first + STEP_LINE[IdxStep];
            int NewCol = CurrentTile.second + STEP_COL[IdxStep];
            if ((0 <= NewLine) && (NewLine < MaxLine) && (0 <= NewCol) && (NewCol < MaxCol))
            {
                if (Floor[NewLine][NewCol] == 1)
                {
                    FillQueue.push({NewLine, NewCol});
                    Floor[NewLine][NewCol] = 0;
                }
            }
        }
    }

    // Switch the border elements from 2 to 1 
    cout << "---- Switching ----" << endl;
    for (int IdxLine = 0; IdxLine < MaxLine; IdxLine++)
    {
        for (int IdxCol = 0; IdxCol < MaxCol; IdxCol++)
        {
            if (Floor[IdxLine][IdxCol] == 2)
            {
                Floor[IdxLine][IdxCol] = 1;
            }
        }
    }

    // Calculate the sum of red / green tiles
    cout << "---- Calculating partial sums ----" << endl;
    vector<vector<long long>> PartialSumTiles(MaxLine, vector<long long>(MaxCol, 0));
    PartialSumTiles[0][0] = Floor[0][0];

    //      Top border
    for (int IdxCol = 1; IdxCol < MaxCol; IdxCol++)
    {
        PartialSumTiles[0][IdxCol] = PartialSumTiles[0][IdxCol - 1] + Floor[0][IdxCol];
    }
    //      Left border
    for (int IdxLine = 1; IdxLine < MaxLine; IdxLine++)
    {
        PartialSumTiles[IdxLine][0] = PartialSumTiles[IdxLine - 1][0] + Floor[IdxLine][0];
    }
    //      Inner part
    for (int IdxLine = 1; IdxLine < MaxLine; IdxLine++)
    {
        for (int IdxCol = 1; IdxCol < MaxCol; IdxCol++)
        {
            PartialSumTiles[IdxLine][IdxCol] = PartialSumTiles[IdxLine - 1][IdxCol] + 
                                               PartialSumTiles[IdxLine][IdxCol - 1] - 
                                               PartialSumTiles[IdxLine - 1][IdxCol - 1] + 
                                               Floor[IdxLine][IdxCol];
        }
    }
    
    // Iterate over the red tiles pairs and find the maximum rectangle
    cout << "---- Finding maximum rectangle ----" << endl;
    long long MaxNumTilesInside = 0;
    for (int IdxRedTile1 = 0; IdxRedTile1 < RedTiles.size() - 1; IdxRedTile1++)
    {
        for (int IdxRedTile2 = IdxRedTile1 + 1; IdxRedTile2 < RedTiles.size(); IdxRedTile2++)
        {
            int FirstMax = max(RedTiles[IdxRedTile1].first, RedTiles[IdxRedTile2].first);
            int FirstMin = min(RedTiles[IdxRedTile1].first, RedTiles[IdxRedTile2].first);
            int SecondMax = max(RedTiles[IdxRedTile1].second, RedTiles[IdxRedTile2].second);
            int SecondMin = min(RedTiles[IdxRedTile1].second, RedTiles[IdxRedTile2].second);
            int DiffLine = FirstMax - FirstMin + 1;
            int DiffCol = SecondMax - SecondMin + 1;
            long long Area = (long long)DiffLine * DiffCol;
            long long NumTilesInside = PartialSumTiles[FirstMax][SecondMax] - 
                                       PartialSumTiles[FirstMax][SecondMin - 1] - 
                                       PartialSumTiles[FirstMin - 1][SecondMax] + 
                                       PartialSumTiles[FirstMin - 1][SecondMin - 1];
            if ((Area == NumTilesInside) && (NumTilesInside > MaxNumTilesInside))
            {
                MaxNumTilesInside = NumTilesInside;
            }
        }
    }

    return MaxNumTilesInside;
}

int main()
{
    vector<pair<int, int>> RedTiles;
    RedTiles = ReadFileLineByLine("input/day9.txt");

    cout << SolvePart1(RedTiles) << endl;

    auto start = chrono::high_resolution_clock::now();
    cout << SolvePart2(RedTiles) << endl;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    cout << "Time taken by function: "<< duration.count() << " minutes" << endl;

    return 0;
}