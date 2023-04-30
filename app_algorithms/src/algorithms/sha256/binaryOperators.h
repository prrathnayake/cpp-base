#pragma once
#include <iostream>

void sizeValidate(std::string *binary1, std::string *binary2);

void size32Validate(std::string *binary);

std::string AND(std::string binary1, std::string binary2);

std::string OR(std::string binary1, std::string binary2);

std::string XNOR(std::string binary1, std::string binary2);

std::string XOR(std::string binary1, std::string binary2);

std::string NOT(std::string binary);