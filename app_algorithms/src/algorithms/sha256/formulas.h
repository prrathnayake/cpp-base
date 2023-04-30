#pragma once
#include <iostream>
#include <bitset>
#include <cmath>

#include "binaryOperators.h"
#include "common.h"
#include "conversions.h"

std::string ShaR(std::string binary, int value)
{
    std::string add = "";

    if (binary.size() > value)
    {
        binary.erase(binary.size() - (value), value);

        for (int i = 0; i < value; i++)
        {
            add += "0";
        }
        return add + binary;
    }
    else
    {
        for (int i = 0; i < value; i++)
        {
            add += "0";
        }
        return add;
    }
}

std::string RotR(std::string binary, int value)
{
    char buffer[value];
    if (binary.size() < value)
    {
        value = value % binary.size();
    }
    else if (binary.size() == value)
    {
        return binary;
    }
    binary.copy(buffer, value, binary.size() - (value));
    binary.erase(binary.size() - (value), value);
    binary = std::string(buffer, value) + binary;

    return binary;
}

std::string additionModulo(std::string x, std::string y)
{
    std::bitset<32> bits1(x);
    unsigned long xInt = bits1.to_ulong();

    std::bitset<32> bits2(y);
    unsigned long yInt = bits2.to_ulong();

    unsigned long result = (xInt + yInt) % 4294967296;

    std::string binary = decimalToBinary(result);
    size32Validate(&binary);

    return binary;
}

std::string Ch(std::string x, std::string y, std::string z)
{
    return XOR(AND(x, y), AND(NOT(x), z));
}

std::string Maj(std::string x, std::string y, std::string z)
{
    return XOR(XOR(AND(x, y), AND(x, z)), AND(y, z));
}

std::string Sigma0(std::string binary)
{
    return XOR(XOR(RotR(binary, 2), RotR(binary, 13)), RotR(binary, 22));
}

std::string Sigma1(std::string binary)
{
    return XOR(XOR(RotR(binary, 6), RotR(binary, 11)), RotR(binary, 25));
}

std::string sigma0(std::string binary)
{
    return XOR(XOR(RotR(binary, 7), RotR(binary, 18)), ShaR(binary, 3));
}

std::string sigma1(std::string binary)
{
    return XOR(XOR(RotR(binary, 17), RotR(binary, 19)), ShaR(binary, 10));
}