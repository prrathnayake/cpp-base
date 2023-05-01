#pragma once
#include <iostream>
#include <sstream>
#include <bitset>
#include <map>

namespace algorithms
{
    std::string decimalToBinary(int n);

    std::string decimalToBinary(unsigned long n);

    std::string hexToBinary(std::string h);

    std::string binaryToHex(std::string binaryString);
}
