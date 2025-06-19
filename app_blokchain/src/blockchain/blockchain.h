#pragma once
#include <iostream>
#include <vector>

#include "block/block.h"

namespace blockchain
{
    class Blockchain
    {
    private:
        std::string hash;
        unsigned int blockCount;
        std::vector<blockchain::Block> blocks;

    public:
        Blockchain();
        void printBlockchain();
    };
}