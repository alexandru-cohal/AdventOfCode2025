#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

vector<pair<long long, long long>> ReadFile(string Filename)
{
    string Element;
    ifstream FileInput(Filename);
    vector<pair<long long, long long>> InputPairs;
    pair<long long, long long> SinglePair;

    while (getline(FileInput, Element, '-'))
    {
        SinglePair.first = stoll(Element);
        getline(FileInput, Element, ',');
        SinglePair.second = stoll(Element);
        InputPairs.push_back(SinglePair);
    }

    return InputPairs;
}

long long SolvePart1(vector<pair<long long, long long>> InputPairs)
{
    long long TotalSum = 0;

    for (int Index = 0; Index < InputPairs.size(); Index++)
    {
        for (long long Element = InputPairs[Index].first; Element <= InputPairs[Index].second; Element++)
        {
            string ElementString = to_string(Element);
            if (ElementString.length() % 2 == 0)
            {
                string FirstHalf = ElementString.substr(0, ElementString.length() / 2);
                string SecondHalf = ElementString.substr(ElementString.length() / 2, ElementString.length() / 2);
                if (FirstHalf == SecondHalf)
                {
                    TotalSum += Element;
                }
            }
        }
    }

    return TotalSum;
}

long long SolvePart2(vector<pair<long long, long long>> InputPairs)
{
    long long TotalSum = 0;

    for (int Index = 0; Index < InputPairs.size(); Index++)
    {
        for (long long Element = InputPairs[Index].first; Element <= InputPairs[Index].second; Element++)
        {
            string ElementString = to_string(Element);
            for (int LengthSequence = 1; LengthSequence <= ElementString.length() / 2; LengthSequence++)
            {
                if (ElementString.length() % LengthSequence == 0)
                {
                    bool ElementIsOk = true;
                    string PreviousSequence = ElementString.substr(0, LengthSequence);
                    for (int IndexStartSequence = LengthSequence; IndexStartSequence < ElementString.length(); IndexStartSequence += LengthSequence)
                    {
                        string CurrentSequence = ElementString.substr(IndexStartSequence, LengthSequence);
                        if (PreviousSequence != CurrentSequence)
                        {
                            ElementIsOk = false;
                            break;
                        }
                    }
                    if (ElementIsOk == true)
                    {
                        TotalSum += Element;
                        break;
                    }
                }
            }
        }
    }

    return TotalSum;
}

int main()
{
    vector<pair<long long, long long>> InputPairs;
    
    InputPairs = ReadFile("input/day2.txt");

    cout << SolvePart1(InputPairs) << endl;
    cout << SolvePart2(InputPairs) << endl;

    return 0;
}