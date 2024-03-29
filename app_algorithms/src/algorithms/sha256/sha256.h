#pragma once
#include <iostream>
#include <vector>

namespace algorithms
{
    std::string padding(std::string binary);

    std::vector<std::vector<std::string>> blockDecomposition(std::string binary);

    std::string hashComputation(std::string binary);
}
