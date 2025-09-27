#include <vector>
#include <openssl/sha.h>

#include "blockchain.h"
#include "block/block.h"
#include "transtraction.h"
#include "utils/log/singletonLogger.h"

blockchain::Blockchain::Blockchain(){
    blockchain::Block genesisBlock;
    blocks.push_back(genesisBlock);
}

void blockchain::Blockchain::printBlockchain()
{
    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::INFO,
        "\nBlockchain\n===========\n",
        __FILE__,
        __LINE__,
        __func__);
    for (int i = 0; i < blocks.size(); i++)
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Block : " + std::to_string(i) + "\n" + blocks[i].toString(),
            __FILE__,
            __LINE__,
            __func__);
    }
}



