#include <iostream>
#include <vector>
#include <openssl/sha.h>

#include "blockchain.h"
#include "block/block.h"
#include "transtraction.h"

blockchain::Blockchain::Blockchain(){
    blockchain::Block genesisBlock;
    blocks.push_back(genesisBlock);
}

void blockchain::Blockchain::printBlockchain()
{
    std::cout << "\nBlockchain\n";
    std::cout << "===========\n\n";
    for (int i = 0; i < blocks.size(); i++)
    {
        std::cout << "Block : " << i << "\n";
        std::cout << blocks[i].toString() << "\n";
    }
}



