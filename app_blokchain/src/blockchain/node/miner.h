#pragma once
#include <iostream>
#include <map>

#include "../block/block.h"
#include "node.h"

namespace blockchain
{
    class Miner : public Node
    {
        void onTranstractionsFromMempool();
        std::string mine(blockchain::Block block);
        std::string getHash(std::string data);
        bool isValidateSignature(blockchain::Transtraction transtraction);
        void sendBlockToBlockchain(blockchain::Block block);
    };
}