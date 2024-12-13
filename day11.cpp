// * Every time you blink, the stones each simultaneously change according to the first applicable rule in this list:
// 
// - If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
// - If the stone is engraved with a number that has an even number of digits, it is replaced by two stones.
//		The left half of the digits are engraved on the new left stone,
//		and the right half of the digits are engraved on the new right stone.
//		(The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
// - If none of the other rules apply, the stone is replaced by a new stone;
//		the old stone's number multiplied by 2024 is engraved on the new stone.
// 
// * Find number of elements after:
//      One star    - 25 blinks
//      Two stars   - 75 blinks
// 
// * Puzzle input: 4329 385 0 1444386 600463 19 1 56615

#include <iostream>
#include <vector>
#include <climits>
#include <chrono>
#include <unordered_map>

using bigUInt = unsigned long long int;
using uInt = unsigned int;

// Used to get power of 10 quickly
static bigUInt pow10[20] = {
        1,                  10,                  100,                  1000,                  10000,
        // ===============
        100000,             1000000,             10000000,             100000000,             1000000000,
        // ===============
        10000000000,        100000000000,        1000000000000,        10000000000000,        100000000000000,
        // ===============
        1000000000000000,   10000000000000000,   100000000000000000,   1000000000000000000,   10000000000000000000
};

static void PrintVector(const std::vector<bigUInt>& numbers)
{
    for (bigUInt n : numbers)
        std::cout << n << " ";
    std::cout << "\n";
}

static uInt CalcNumOfDigits(bigUInt value)
{
    uInt ret = 0;
    do
    {
        ++ret;
        value /= 10;
    } while (value > 0);

    return ret;
}

// If value has an even number of digits it splits it into 2 numbers
// EX:
// 2024 => leftOut = 20, rightOut = 24
static bool TrySplit(bigUInt value, bigUInt& leftOut, bigUInt& rightOut)
{
    uInt numDig = CalcNumOfDigits(value);
    if (numDig % 2 == 1) return false;

    bigUInt halfPow10 = pow10[numDig / 2];
    rightOut = value % halfPow10;
    leftOut = value / halfPow10;
    return true;
}

// For every number (bigUInt) we keep a map of number of elements per x number of blinks
// EX:
// Key: 0 (bigUInt)
// Num of blinks (uInt), Num of elements (bigUInt), Resulting array
// 1                   , 1                       => { 1 }
// 2                   , 1                       => { 2024 }
// 3                   , 2                       => { 20, 24 }
// 4                   , 4                       => { 2, 0, 2, 4 }
static std::unordered_map<bigUInt, std::unordered_map<uInt, bigUInt>> g_BlinkCache;

// Keeps track of how many times the needed value was found in g_BlinkCache
static uInt g_CacheHit = 0;

// If an entry for num and blinks doesn't exist already we create a new one with value 0
static std::unordered_map<uInt, bigUInt>::iterator ProvideCacheEntry(bigUInt num, uInt blinks)
{
    std::unordered_map<uInt, bigUInt>::iterator entryIt;
    auto it1 = g_BlinkCache.find(num);
    if (it1 != g_BlinkCache.end())
    {
        std::unordered_map<uInt, bigUInt>& blinkMap = it1->second;
        auto it2 = blinkMap.find(blinks);
        if (it2 != blinkMap.end())
        {
            // Entry found
            return it2;
        }
        else
        {
            // Add empty entry for this number of blinks
            // Insert returns std::pair<Iterator, bool>
            return blinkMap.insert({ blinks, {0} }).first;
        }
    }

    // Add empty map for this number
    // Insert returns std::pair<Iterator, bool>
    auto insertIt = g_BlinkCache.insert({ num, {} }).first;
    std::unordered_map<uInt, bigUInt>& blinkMap = insertIt->second;

    // Add empty entry for this number of blinks
    // Insert returns std::pair<Iterator, bool>
    return blinkMap.insert({ blinks, {0} }).first;
}

// BlinkRec is called recursively and returns number of elements in the array
// after blinking "blinks" times for the starting number "num" 
static bigUInt BlinkRec(bigUInt num, uInt blinks)
{
    // Recursion end condition
    if (blinks == 0) return 1;

    // Check in cache first
    std::unordered_map<uInt, bigUInt>::iterator entryIt = ProvideCacheEntry(num, blinks);
    bigUInt cachedVal = entryIt->second;
    if (cachedVal != 0)
    {
        ++g_CacheHit;
        return cachedVal;
    }

    // Continue recursion
    bigUInt retVal = 0;
    if (num == 0)
    {
        retVal = BlinkRec(1, blinks - 1);
    }
    else
    {
        bigUInt left, right;
        if (TrySplit(num, left, right))
        {
            retVal = BlinkRec(left, blinks - 1) + BlinkRec(right, blinks - 1);
        }
        else
        {
            retVal = BlinkRec(num * 2024, blinks - 1);
        }
    }

    // Update cache value and return
    entryIt->second = retVal;
    return retVal;
}

void Day11()
{
    constexpr uInt BLINK_NUM = 75;
    const std::vector<bigUInt> stones{ 4329, 385, 0, 1444386, 600463, 19, 1, 56615 };

    std::cout << "Day 11 puzzle: " << std::endl;

    // Number of elements in the array
    // after blinking "BLINK_NUM" times
    bigUInt sum = 0;

    auto start = std::chrono::steady_clock::now(); // START TIMESTAMP
    for (bigUInt n : stones)
    {
        sum += BlinkRec(n, BLINK_NUM);
    }
    auto end = std::chrono::steady_clock::now(); // END TIMESTAMP

    std::cout << "Result: " << sum << std::endl;
    std::cout << "Cache was used for a total of: " << g_CacheHit << " times" << std::endl;

    std::cout << "Simulation took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}