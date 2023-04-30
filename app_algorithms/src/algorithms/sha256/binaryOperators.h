#pragma once
#include <iostream>

void sizeValidate(std::string *binary1, std::string *binary2)
{
    std::string add;
    int bin1Len = binary1->size();
    int bin2Len = binary2->size();

    if (bin1Len < bin2Len)
    {
        for (int i = 0; i < (bin2Len - bin1Len); i++)
        {
            add += "0";
        }
        *binary1 = add + *binary1;
    }
    else if (bin2Len < bin1Len)
    {
        for (int i = 0; i < (bin1Len - bin2Len); i++)
        {
            add += "0";
        }
        *binary2 = add + *binary2;
    }
}

void size32Validate(std::string *binary)
{
    std::string add;
    int len = binary->size();
    if (binary->size() < 32)
    {
        for (int i = 0; i < (32 - len); i++)
        {
            add += "0";
        }
        *binary = add + *binary;
    }
}

std::string AND(std::string binary1, std::string binary2)
{
    std::string result;

    for (int i = 0; i < binary1.size(); i++)
    {
        if (binary1[i] == '1' & binary2[i] == '1')
        {
            result += '1';
        }
        else
        {
            result += '0';
        }
    }

    return result;
}

std::string OR(std::string binary1, std::string binary2)
{
    std::string result;

    for (int i = 0; i < binary1.size(); i++)
    {
        if (binary1[i] == '1' || binary2[i] == '1')
        {
            result += '1';
        }
        else
        {
            result += '0';
        }
    }

    return result;
}

std::string XNOR(std::string binary1, std::string binary2)
{
    std::string result;

    for (int i = 0; i < binary1.size(); i++)
    {
        if (binary1[i] == binary2[i])
        {
            result += '1';
        }
        else
        {
            result += '0';
        }
    }

    return result;
}

std::string XOR(std::string binary1, std::string binary2)
{
    std::string result = "";

    for (int i = 0; i < binary1.size(); i++)
    {
        if (binary1[i] == binary2[i])
        {
            result += '0';
        }
        else
        {
            result += '1';
        }
    }

    return result;
}

std::string NOT(std::string binary)
{
    std::string result;

    for (int i = 0; i < binary.size(); i++)
    {
        if (binary[i] == '1')
        {
            result += '0';
        }
        else
        {
            result += '1';
        }
    }

    return result;
}