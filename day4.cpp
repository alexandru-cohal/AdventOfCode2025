#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const int STEP_LINE[8] = {0, -1, -1, -1, 0, 1, 1, 1};
const int STEP_COL[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; 

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

int SolvePart1(vector<string> WallMap)
{
    int NumAccessibleRolls = 0;
    int NumLines = WallMap.size();
    int NumCols = WallMap[0].length();

    for (int IndexLine = 0; IndexLine < NumLines; IndexLine++)
    {
        for (int IndexCol = 0; IndexCol < NumCols; IndexCol++)
        {
            if (WallMap[IndexLine][IndexCol] == '@')
            {
                int NumNeighbouringRolls = 0;
                bool RollOk = true;

                for (int IndexDirection = 0; IndexDirection < 8; IndexDirection++)
                {
                    if ((0 <= IndexLine + STEP_LINE[IndexDirection]) && (IndexLine + STEP_LINE[IndexDirection] < NumLines) &&
                        (0 <= IndexCol + STEP_COL[IndexDirection]) && (IndexCol + STEP_COL[IndexDirection] < NumCols))
                    {
                        if (WallMap[IndexLine + STEP_LINE[IndexDirection]][IndexCol + STEP_COL[IndexDirection]] == '@')
                        {
                            NumNeighbouringRolls++;
                            if (NumNeighbouringRolls == 4)
                            {
                                RollOk = false;
                                break;
                            }
                        }
                    }
                }
                
                if (RollOk == true)
                {
                    NumAccessibleRolls++;
                }
            }
        }
    }

    return NumAccessibleRolls;
}

int SolvePart2(vector<string> WallMap)
{
    int NumAccessibleRolls = 0;
    vector<pair<int, int>> AccessibleRolls;
    int NumLines = WallMap.size();
    int NumCols = WallMap[0].length();

    do
    {
        AccessibleRolls.clear();

        for (int IndexLine = 0; IndexLine < NumLines; IndexLine++)
        {
            for (int IndexCol = 0; IndexCol < NumCols; IndexCol++)
            {
                if (WallMap[IndexLine][IndexCol] == '@')
                {
                    int NumNeighbouringRolls = 0;
                    bool RollOk = true;

                    for (int IndexDirection = 0; IndexDirection < 8; IndexDirection++)
                    {
                        if ((0 <= IndexLine + STEP_LINE[IndexDirection]) && (IndexLine + STEP_LINE[IndexDirection] < NumLines) &&
                            (0 <= IndexCol + STEP_COL[IndexDirection]) && (IndexCol + STEP_COL[IndexDirection] < NumCols))
                        {
                            if (WallMap[IndexLine + STEP_LINE[IndexDirection]][IndexCol + STEP_COL[IndexDirection]] == '@')
                            {
                                NumNeighbouringRolls++;
                                if (NumNeighbouringRolls == 4)
                                {
                                    RollOk = false;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (RollOk == true)
                    {
                        NumAccessibleRolls++;
                        AccessibleRolls.push_back({IndexLine, IndexCol});
                    }
                }
            }
        }

        for (int IndexRoll = 0; IndexRoll < AccessibleRolls.size(); IndexRoll++)
        {
            WallMap[AccessibleRolls[IndexRoll].first][AccessibleRolls[IndexRoll].second] = '.';
        }
    } 
    while (AccessibleRolls.size() != 0);

    return NumAccessibleRolls;
}

int main()
{
    vector<string> WallMap;
    WallMap = ReadFileLineByLine("input/day4.txt");

    cout << SolvePart1(WallMap) << endl;
    cout << SolvePart2(WallMap) << endl;

    return 0;
}
