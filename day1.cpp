// * Pair up the smallest number in the left list with the smallest number
// in the right list, then the second-smallest left number with the second-smallest right number
// 
// - One star:
//      calc num of times elements from the first list appear in the second one
//      and sum those values
// - Two stars:
//      calc similarity score:
//          value of the element from the first list multiplied
//          by the number of times it appears in the second list
// 
// * Puzzle input: day1input.txt

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdlib.h>

using int64 = long long int;
using uInt = unsigned int;
using bigUInt = unsigned long long int;

static void PrintVectors(const std::vector<int64>& vec1, const std::vector<int64>& vec2)
{
    for (uInt i = 0; i < vec1.size(); ++i)
    {
        std::cout << vec1[i] << " " << vec2[i] << std::endl;
    }
}

static bigUInt CalcSimilarity(const std::vector<int64>& vec1, const std::vector<int64>& vec2)
{
    bigUInt similarity = 0;
    for (const int64 num1 : vec1)
    {
        uInt sum = 0;
        for (const int64 num2 : vec2)
        {
            if (num1 == num2)
            {
                ++sum;
            }
        }
        similarity += sum * num1;
    }

    return similarity;
}

static bigUInt SumDistances(const std::vector<int64>& vec1, const std::vector<int64>& vec2)
{
    bigUInt sum = 0;
    for (uInt i = 0; i < vec1.size(); ++i)
    {
        sum += abs(vec1[i] - vec2[i]);
    }

    return sum;
}

void ParseInput(const std::string& filename, std::vector<int64>& outVec1, std::vector<int64>& outVec2)
{
    std::ifstream inFile;
    inFile.open(filename);
    if (!inFile)
    {
        std::cout << "Unable to open file";
        exit(1);
    }

    uInt input1, input2;
    while (inFile >> input1 && inFile >> input2)
    {
        outVec1.push_back(input1);
        outVec2.push_back(input2);
    }

    std::sort(outVec1.begin(), outVec1.end());
    std::sort(outVec2.begin(), outVec2.end());
}

void Day1()
{
    std::cout << "Day 1 puzzle: " << std::endl;

    bigUInt sum = 0;
    std::vector<int64> vec1;
    std::vector<int64> vec2;

    auto start = std::chrono::steady_clock::now(); // START TIMESTAMP

    ParseInput("day1input.txt", vec1, vec2);
    sum = CalcSimilarity(vec1, vec2);

    auto end = std::chrono::steady_clock::now(); // END TIMESTAMP

    std::cout << "Result: " << sum << std::endl;

    std::cout << "Simulation took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

}