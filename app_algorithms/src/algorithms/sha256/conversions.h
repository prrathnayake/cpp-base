#pragma once
#include <iostream>
#include <sstream>
#include <bitset>
#include <map>

std::map<std::string, char> binaryToHexMap = {
    {"0000", '0'},
    {"0001", '1'},
    {"0010", '2'},
    {"0011", '3'},
    {"0100", '4'},
    {"0101", '5'},
    {"0110", '6'},
    {"0111", '7'},
    {"1000", '8'},
    {"1001", '9'},
    {"1010", 'a'},
    {"1011", 'b'},
    {"1100", 'c'},
    {"1101", 'd'},
    {"1110", 'e'},
    {"1111", 'f'}};

std::string decimalToBinary(int n)
{
    std::string binary = "";
    std::string tem = "";

    while (n > 0)
    {
        tem += std::to_string(n % 2);
        n = n / 2;
    }

    for (int i = (tem.size() - 1); i >= 0; i--)
    {
        binary += tem[i];
    }

    return binary;
}

std::string decimalToBinary(unsigned long n)
{
    std::string binary = "";
    std::string tem = "";

    while (n > 0)
    {
        tem += std::to_string(n % 2);
        n = n / 2;
    }

    for (int i = (tem.size() - 1); i >= 0; i--)
    {
        binary += tem[i];
    }

    return binary;
}

std::string hexToBinary(std::string h)
{
    std::stringstream ss;
    ss << std::hex << h;
    unsigned n;
    ss >> n;
    std::bitset<32> b(n);
    return b.to_string();
}

std::string binaryToHex(std::string binaryString)
{
    while (binaryString.size() % 4 != 0)
    {
        binaryString = '0' + binaryString;
    }

    std::string hexString = "";
    for (std::size_t i = 0; i < binaryString.size(); i += 4)
    {
        std::string binarySubstring = binaryString.substr(i, 4);
        hexString += binaryToHexMap[binarySubstring];
    }
    return hexString;
}