#pragma once
#include <iostream>
#include <vector>

#include "block.h"
#include "blockHeader.h"
#include "../transtraction.h"

namespace blockchain
{
    class Block : public BlockHeader
    {
    private:
        std::vector<blockchain::Transtraction> transtractions;

    public:
        Block();
        Block(std::string previousHash, std::vector<blockchain::Transtraction> transtractions, std::string timestamp);
        std::string toString();
        int getNonce();
        void setNonce(int nonce);
    };
}
