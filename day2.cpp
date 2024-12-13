// * Report only counts as safe if both of the following are true:
//
// - The levels are either all increasing or all decreasing.
// - Any two adjacent levels differ by at least one and at most three.
// 
// * Find number of elements after:
//      Two stars   -   an unsafe report can become safe by removing one elements
// 
// * Puzzle input: day2input.txt

#include <iostream>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <sstream>
#include <string>

using int64 = long long int;
using uInt = unsigned int;
using bigUInt = unsigned long long int;

enum class ETrend
{
    Increasing,
    Decreasing,
    Invalid
};

bool CheckSafety(int prevVal, int nextVal, ETrend trend)
{
    int diff = prevVal - nextVal;

    uInt absDiff = std::abs(diff);
    if (absDiff == 0 || absDiff > 3)
    {
        return false;
    }

    switch (trend)
    {
    case ETrend::Increasing:
        return diff < 0;
    case ETrend::Decreasing:
        return diff > 0;
    }

    return true;
}

uInt ParseInput(const std::string& filename)
{
    std::ifstream inFile;
    inFile.open(filename);
    if (!inFile)
    {
        std::cout << "Unable to open file";
        exit(1);
    }

    std::string str;

    uInt safeLines = 0;
    while (std::getline(inFile, str))
    {
        ETrend trend = ETrend::Invalid;
        std::stringstream stream(str);

        int prevValue = -1;
        uInt nextValue = 0;
        bool isSafe = true;
        bool canSkip = true;
        int index = -1;
        while (stream >> nextValue)
        {
            if (prevValue != -1)
            {   
                bool trendWasSet = true;
                if (trend == ETrend::Invalid)
                {
                    trendWasSet = false;
                    trend = nextValue > prevValue ? ETrend::Increasing : ETrend::Decreasing;
                }
                if (!CheckSafety(prevValue, nextValue, trend))
                {
                    if (canSkip)
                    {
                        if (!trendWasSet)
                        {
                            trend = ETrend::Invalid;
                        }

                        prevValue = nextValue;
                        canSkip = false;
                        continue;
                    }
                    isSafe = false;
                    break;
                }
            }

            prevValue = nextValue;
        }
        if (isSafe)
        {
            ++safeLines;
        }
    }
    return safeLines;
}

void Day2()
{
    std::cout << "Day 2 puzzle: " << std::endl;

    bigUInt res = 0;

    auto start = std::chrono::steady_clock::now(); // START TIMESTAMP

    res = ParseInput("day2input.txt");

    auto end = std::chrono::steady_clock::now(); // END TIMESTAMP

    std::cout << "Result: " << res << std::endl;

    std::cout << "Simulation took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}