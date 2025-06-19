#pragma once
#include <iostream>
#include <map>

#include "transtraction.h"

namespace blockchain
{
    class MemPool
    {
    private:
        std::map<int, blockchain::Transtraction> transtractions;

    public:
        void addTranstractionToMemPool(blockchain::Transtraction);
        void removeTranstractionFromMemPool(int TxID);
    };
}