#pragma once
#include <iostream>
#include <vector>

#include "block.h"
#include "database.h"

namespace blockchain
{
    class Blockchain
    {
    private:
        std::string hash;
        std::vector<blockchain::Block> blocks;
        std::vector<blockchain::Database> databases;

    public:
        void addNewDatabase(blockchain::Database database);
        void addNewTranstraction(blockchain::Transtraction transtraction);
        void printData(blockchain::Database database);
        void printBlockchain();
        std::string getHash(std::string data);
        std::string mine(blockchain::Block block);
    };
}