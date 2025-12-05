#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

void ReadFile(string FileName, vector<pair<long long,long long>> &Intervals, vector<long long> &Ids)
{
    ifstream File(FileName);
    string Line;
    bool FirstPart = true;

    while (getline(File, Line))
    {
        if (Line.compare("") != 0)
        {
            if (FirstPart == true)
            {
                int IdxSeparator = Line.find('-');
                Intervals.push_back({stoll(Line.substr(0, IdxSeparator)), 
                                    stoll(Line.substr(IdxSeparator+1, Line.length()-IdxSeparator-1))});
            }
            else
            {
                Ids.push_back(stoll(Line));
            }
        }
        else
        {
            FirstPart = false;
        }
    }
    File.close();
}

int SolvePart1(vector<pair<long long, long long>> Intervals, vector<long long> Ids)
{
    int NumFreshId = 0;

    for (int IndexId = 0; IndexId < Ids.size(); IndexId++)
    {
        for (int IdxIntrv = 0; IdxIntrv < Intervals.size(); IdxIntrv++)
        {
            if ((Intervals[IdxIntrv].first <= Ids[IndexId]) && 
                (Ids[IndexId] <= Intervals[IdxIntrv].second))
            {
                NumFreshId++;
                break;
            }
        }
    }

    return NumFreshId;
}

bool SortCompareFcn(pair<long long, long long> Intrv1, pair<long long, long long> Intrv2)
{
    return (Intrv1.first < Intrv2.first);
}

long long SolvePart2(vector<pair<long long, long long>> Intrvs)
{
    long long NumFreshId = 0;
    vector<pair<long long, long long>> DisIntrvs;

    sort(Intrvs.begin(), Intrvs.end(), SortCompareFcn);

    DisIntrvs.push_back(Intrvs[0]);

    for (int IdxIntrv = 1; IdxIntrv < Intrvs.size(); IdxIntrv++)
    {
        bool DisIntrvUpdate = false;

        for (int IdxDisIntrv = 0; IdxDisIntrv < DisIntrvs.size(); IdxDisIntrv++)
        {
            if ((DisIntrvs[IdxDisIntrv].first <= Intrvs[IdxIntrv].first) && (Intrvs[IdxIntrv].first <= DisIntrvs[IdxDisIntrv].second))
            {
                // Left side of Intrv is inside DisIntrv
                if ((DisIntrvs[IdxDisIntrv].first <= Intrvs[IdxIntrv].second) && (Intrvs[IdxIntrv].second <= DisIntrvs[IdxDisIntrv].second))
                {
                    // Right side of Intrv is inside DisIntrv
                    // Nothing to do
                    DisIntrvUpdate = true;
                    break;
                }
                else
                {
                    // Right side of Intrv is not inside DisIntrv
                    // Extend the right side of DisIntrv
                    DisIntrvs[IdxDisIntrv].second = Intrvs[IdxIntrv].second;
                    DisIntrvUpdate = true;
                    break;
                }
            }
            else
            {
                if ((DisIntrvs[IdxDisIntrv].first <= Intrvs[IdxIntrv].second) && (Intrvs[IdxIntrv].second <= DisIntrvs[IdxDisIntrv].second))
                {
                    // Right side of Intrv is inside DisIntrv
                    // But left side of Intrv is not inside DisIntrv
                    // Extend the left side of DisIntrv
                    DisIntrvs[IdxDisIntrv].first = Intrvs[IdxIntrv].first;
                    DisIntrvUpdate = true;
                    break;
                }
            }
        }
        if (DisIntrvUpdate == false)
        {
            // Intrv is not overlapping with any DisIntrvs
            // Add one more DisIntrvs
            DisIntrvs.push_back(Intrvs[IdxIntrv]);
        }
    }

    for (int IdxDisIntrv = 0; IdxDisIntrv < DisIntrvs.size(); IdxDisIntrv++)
    {
        NumFreshId += (DisIntrvs[IdxDisIntrv].second - DisIntrvs[IdxDisIntrv].first + 1);
    }

    return NumFreshId;
}

int main()
{
    vector<pair<long long, long long>> Intervals;
    vector<long long> Ids;

    ReadFile("input/day5.txt", Intervals, Ids);

    cout << SolvePart1(Intervals, Ids) << endl;
    cout << SolvePart2(Intervals) << endl;

    return 0;
}