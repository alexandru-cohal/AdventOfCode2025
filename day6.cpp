#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void ReadFilePart1(string FileName, vector<vector<int>> &Terms, vector<char> &Operators)
{
    ifstream File(FileName);
    string Line;
    vector<int> TermLine;
    int Term;

    while (getline(File, Line))
    {
        istringstream LineStringStream(Line);
        TermLine.clear();

        while(LineStringStream >> Term)
        {
            TermLine.push_back(Term);
        }

        if (TermLine.size() != 0)
        {
            // Line with terms
            Terms.push_back(TermLine);
        }
        else
        {
            // Line with operators
            for (int IndexChar = 0; IndexChar < Line.size(); IndexChar++)
            {
                if (Line[IndexChar] != ' ')
                {
                    Operators.push_back(Line[IndexChar]);
                }
            }
        }
    }
}

long long SolvePart1(vector<vector<int>> Terms, vector<char> Operators)
{
    long long TotalResults = 0;

    for (int IdxOperator = 0; IdxOperator < Operators.size(); IdxOperator++)
    {
        long long Result = Terms[0][IdxOperator];
        
        for (int IdxTerm = 1; IdxTerm < Terms.size(); IdxTerm++)
        {
            if (Operators[IdxOperator] == '+')
            {
                Result += Terms[IdxTerm][IdxOperator];
            }
            else
            {
                Result *= Terms[IdxTerm][IdxOperator];
            }
        } 

        TotalResults += Result;
    }

    return TotalResults;
}

void ReadFilePart2(string FileName, vector<int> &Terms, vector<char> &Operators)
{
    ifstream File(FileName);
    string Line;
    int IdxLine = -1;

    while (getline(File, Line))
    {
        if ((Line.find('+') == string::npos) && (Line.find('+') == string::npos))
        {
            // Line with digits of terms
            IdxLine++;
            for (int IndexChar = 0; IndexChar < Line.length(); IndexChar++)
            {
                if (IdxLine > 0)
                {
                    // Not the first line with digits of terms
                    if (Line[IndexChar] != ' ')
                    {
                        Terms[IndexChar] = Terms[IndexChar] * 10 + (Line[IndexChar] - '0');
                    }
                    else
                    {
                    }
                }
                else
                {
                    // First line with digits of terms
                    if (Line[IndexChar] != ' ')
                    {
                        Terms.push_back(Line[IndexChar] - '0');
                    }
                    else
                    {
                        Terms.push_back(0);
                    }
                }
            }
        }
        else
        {
            // Line with operators
            for (int IndexChar = 0; IndexChar < Line.size(); IndexChar++)
            {
                if (Line[IndexChar] != ' ')
                {
                    Operators.push_back(Line[IndexChar]);
                }
            }
        }
    }
}

long long SolvePart2(vector<int> Terms, vector<char> Operators)
{
    long long TotalResults = 0;
    int IdxOperator = 0;
    long long Result = (Operators[IdxOperator] == '+') ? 0 : 1;

    // Add a 0 for iterating the following loop one more time and add the result of the last operation
    Terms.push_back(0);
        
    for (int IdxTerm = 0; IdxTerm < Terms.size(); IdxTerm++)
    {
        if (Terms[IdxTerm] != 0)
        {
            if (Operators[IdxOperator] == '+')
            {
                Result += Terms[IdxTerm];
            }
            else
            {
                Result *= Terms[IdxTerm];
            }
        }
        else
        {
            TotalResults += Result;
            IdxOperator++;
            Result = (Operators[IdxOperator] == '+') ? 0 : 1;
        }
    } 

    return TotalResults;
}

int main()
{
    vector<vector<int>> TermsPart1;
    vector<char> OperatorsPart1;

    ReadFilePart1("input/day6.txt", TermsPart1, OperatorsPart1);
    cout << SolvePart1(TermsPart1, OperatorsPart1) << endl;

    vector<int> TermsPart2;
    vector<char>OperatorsPart2;

    ReadFilePart2("input/day6.txt", TermsPart2, OperatorsPart2);
    cout << SolvePart2(TermsPart2, OperatorsPart2) << endl;

    return 0;
}