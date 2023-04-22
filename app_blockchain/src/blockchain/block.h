#pragma once
#include <iostream>

#include "block.h"
#include "database.h"
#include "transtraction.h"

namespace blockchain
{
    class Block
    {
    private:
        int nonce = 0;
        std::string preHash;
        blockchain::Transtraction trans;
        std::string ts;

    public:
        Block(std::string previousHash, blockchain::Transtraction transtraction, std::string timestamp);
        void generateHash();
        std::string toString();
        std::string toStringWithTranstraction();
    };
}
