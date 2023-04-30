#include <iostream>
#include <bitset>

#include "common.h"

std::string toBinary(std::string const &str)
{
    std::string binary = "";
    for (char const &c : str)
    {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}