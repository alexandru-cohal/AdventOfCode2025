#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

const int NUM_CONNECTIONS = 1000;

typedef struct BoxType
{
    int X, Y, Z;
    int IdxCircuit;
} BoxType;

typedef struct DistanceType
{
    int IdxBox1, IdxBox2;
    double Distance;
} DistanceType;

vector<BoxType> ReadFile(string Filename)
{
    ifstream FileInput(Filename);
    string Coordinate;
    BoxType CurrentBox;
    vector<BoxType> Boxes;

    while (getline(FileInput, Coordinate, ','))
    {
        CurrentBox.IdxCircuit = 0;
        CurrentBox.X = stoi(Coordinate);
        getline(FileInput, Coordinate, ',');
        CurrentBox.Y = stoi(Coordinate);
        getline(FileInput, Coordinate);
        CurrentBox.Z = stoi(Coordinate);
        Boxes.push_back(CurrentBox);
    }

    return Boxes;
}

double CalculateDistance(BoxType Box1, BoxType Box2)
{
    return sqrt(pow((Box1.X - Box2.X), 2) + 
                pow((Box1.Y - Box2.Y), 2) + 
                pow((Box1.Z - Box2.Z), 2));
}

bool SortCompareFcnDistances(DistanceType Dist1, DistanceType Dist2)
{
    return Dist1.Distance < Dist2.Distance;
}

bool SortCompareFcnCircuitSizes(int Size1, int Size2)
{
    return Size1 > Size2;
}

long long SolvePart1(vector<BoxType> Boxes)
{
    vector<DistanceType> Distances;

    // Calculate the distances between boxes and sort them in ascending order
    for (int IdxBox1 = 0; IdxBox1 < Boxes.size() - 1; IdxBox1++)
    {
        for (int IdxBox2 = IdxBox1 + 1; IdxBox2 < Boxes.size(); IdxBox2++)
        {
            DistanceType CurrentDistance;
            CurrentDistance.IdxBox1 = IdxBox1;
            CurrentDistance.IdxBox2 = IdxBox2;
            CurrentDistance.Distance = CalculateDistance(Boxes[IdxBox1], Boxes[IdxBox2]);
            Distances.push_back(CurrentDistance);
        }
    }

    sort(Distances.begin(), Distances.end(), SortCompareFcnDistances);

    // Build the circuits
    vector<int> CircuitSizes;
    for (int IdxDistance = 0; IdxDistance < NUM_CONNECTIONS; IdxDistance++)
    {
        int IdxCircuitBox1 = Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit;
        int IdxCircuitBox2 = Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit;
        if ((IdxCircuitBox1 == 0) && (IdxCircuitBox2 == 0))
        {
            // None of them are in a circuit
            CircuitSizes.push_back(2);
            Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit = CircuitSizes.size();
            Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit = CircuitSizes.size();
        }
        else if (IdxCircuitBox1 == 0)
        {
            // The first one is not in a circuit but the second one is
            CircuitSizes[IdxCircuitBox2 - 1]++;
            Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit = Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit;
        }
        else if (IdxCircuitBox2 == 0)
        {
            // The second one is not in a circuit but the first one is
            CircuitSizes[IdxCircuitBox1 - 1]++;
            Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit = Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit;
        }
        else if (IdxCircuitBox1 != IdxCircuitBox2)
        {
            // Both of them are in circuits but they are different
            // Join the 2 circuits => Switch the IdxCircuit of the second one into the IdxCircuit of the first one
            CircuitSizes[IdxCircuitBox1 - 1] += CircuitSizes[IdxCircuitBox2 - 1];
            CircuitSizes[IdxCircuitBox2 - 1] = 0;
            for (int IdxBox = 0; IdxBox < Boxes.size(); IdxBox++)
            {
                if (Boxes[IdxBox].IdxCircuit == IdxCircuitBox2)
                {
                    Boxes[IdxBox].IdxCircuit = IdxCircuitBox1;
                }
            }
        }
    }

    // Sort the circuit sizes in descending order
    sort(CircuitSizes.begin(), CircuitSizes.end(), SortCompareFcnCircuitSizes);

    return CircuitSizes[0] * CircuitSizes[1] * CircuitSizes[2];
}

long long SolvePart2(vector<BoxType> Boxes)
{
    vector<DistanceType> Distances;
    long long WallDistance = 0;

    // Calculate the distances between boxes and sort them in ascending order
    for (int IdxBox1 = 0; IdxBox1 < Boxes.size() - 1; IdxBox1++)
    {
        for (int IdxBox2 = IdxBox1 + 1; IdxBox2 < Boxes.size(); IdxBox2++)
        {
            DistanceType CurrentDistance;
            CurrentDistance.IdxBox1 = IdxBox1;
            CurrentDistance.IdxBox2 = IdxBox2;
            CurrentDistance.Distance = CalculateDistance(Boxes[IdxBox1], Boxes[IdxBox2]);
            Distances.push_back(CurrentDistance);
        }
    }

    sort(Distances.begin(), Distances.end(), SortCompareFcnDistances);

    // Build the circuits
    vector<int> CircuitSizes;
    for (int IdxDistance = 0; IdxDistance < Distances.size(); IdxDistance++)
    {
        int IdxCircuitBox1 = Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit;
        int IdxCircuitBox2 = Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit;
        if ((IdxCircuitBox1 == 0) && (IdxCircuitBox2 == 0))
        {
            // None of them are in a circuit
            CircuitSizes.push_back(2);
            Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit = CircuitSizes.size();
            Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit = CircuitSizes.size();
        }
        else if (IdxCircuitBox1 == 0)
        {
            // The first one is not in a circuit but the second one is
            CircuitSizes[IdxCircuitBox2 - 1]++;
            Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit = Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit;

            // Check if only one circuit with all the boxes was obtained
            if (CircuitSizes[IdxCircuitBox2 - 1] == Boxes.size())
            {
                WallDistance = (long long)Boxes[Distances[IdxDistance].IdxBox1].X * 
                                          Boxes[Distances[IdxDistance].IdxBox2].X;
                break;
            }
        }
        else if (IdxCircuitBox2 == 0)
        {
            // The second one is not in a circuit but the first one is
            CircuitSizes[IdxCircuitBox1 - 1]++;
            Boxes[Distances[IdxDistance].IdxBox2].IdxCircuit = Boxes[Distances[IdxDistance].IdxBox1].IdxCircuit;

            // Check if only one circuit with all the boxes was obtained
            if (CircuitSizes[IdxCircuitBox1 - 1] == Boxes.size())
            {
                WallDistance = (long long)Boxes[Distances[IdxDistance].IdxBox1].X * 
                                          Boxes[Distances[IdxDistance].IdxBox2].X;
                break;
            }
        }
        else if (IdxCircuitBox1 != IdxCircuitBox2)
        {
            // Both of them are in circuits but they are different
            // Join the 2 circuits => Switch the IdxCircuit of the second one into the IdxCircuit of the first one
            CircuitSizes[IdxCircuitBox1 - 1] += CircuitSizes[IdxCircuitBox2 - 1];
            CircuitSizes[IdxCircuitBox2 - 1] = 0;
            for (int IdxBox = 0; IdxBox < Boxes.size(); IdxBox++)
            {
                if (Boxes[IdxBox].IdxCircuit == IdxCircuitBox2)
                {
                    Boxes[IdxBox].IdxCircuit = IdxCircuitBox1;
                }
            }

            // Check if only one circuit with all the boxes was obtained
            if (CircuitSizes[IdxCircuitBox1 - 1] == Boxes.size())
            {
                WallDistance = (long long)Boxes[Distances[IdxDistance].IdxBox1].X * 
                                          Boxes[Distances[IdxDistance].IdxBox2].X;
                break;
            }
        }
    }

    return WallDistance;
}

int main()
{
    vector<BoxType> Boxes;
    Boxes = ReadFile("input/day8.txt");

    cout << SolvePart1(Boxes) << endl;
    cout << SolvePart2(Boxes) << endl;

    return 0;
}