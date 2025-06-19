#pragma once
#include <iostream>
#include <vector>

namespace blockchain
{
    class BlockHeader
    {
    public:
        int nonce = 0;
        std::string previousHash;
        std::string timestamp ;
        std::string merkleRoot;
    };
}