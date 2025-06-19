#pragma once
#include <iostream>

#include "../blockchain.h"
#include "../transtraction.h"
#include "../memPool.h"

namespace blockchain
{
    class Node
    {
    private:
        std::string privateKey;

    public:
        std::string publicKey;
        std::string name;
        blockchain::MemPool memPool;
        blockchain::Blockchain blockchain;
        Node();
        void createTranstraction(std::string to, float amount);
    };
}