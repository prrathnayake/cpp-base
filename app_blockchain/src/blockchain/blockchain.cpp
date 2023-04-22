#include <iostream>
#include <vector>
#include <openssl/sha.h>

#include "blockchain.h"
#include "block.h"
#include "transtraction.h"
#include "database.h"

void blockchain::Blockchain::addNewDatabase(blockchain::Database database)
{
    databases.push_back(database);
}

void blockchain::Blockchain::addNewTranstraction(blockchain::Transtraction transtraction)
{
    if (blocks.size() == 0)
    {
        blockchain::Transtraction trans("privateKey", "publicKey", "key", "0");
        blockchain::Block bl("preHash", trans, "timestamp");
        blocks.push_back(bl);

        for (int i = 0; i < databases.size(); i++)
        {
            databases[i].addData("key", "0");
        }
        hash = getHash(trans.toString());
    }

    blockchain::Block block(hash, transtraction, "timestamp");
    blocks.push_back(block);
    for (int i = 0; i < databases.size(); i++)
    {
        databases[i].addData(transtraction.getKey(), transtraction.getValue());
    }
    hash = getHash(transtraction.toString());
}

void blockchain::Blockchain::printData(blockchain::Database database)
{
    for (int i = 0; i < databases.size(); i++)
    {
        databases[i].printData();
    }
}

void blockchain::Blockchain::printBlockchain()
{
    for (int i = 0; i < blocks.size(); i++)
    {
        std::cout << "Block : " << i << "\n";
        std::cout << blocks[i].toStringWithTranstraction() << "\n";
    }
}

std::string blockchain::Blockchain::getHash(std::string data)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

    // Convert the hash to a hexadecimal string
    std::string hex_hash;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        char hex_byte[3];
        std::sprintf(hex_byte, "%02x", hash[i]);
        hex_hash += hex_byte;
    }

    // Print the hash
    return hex_hash;
}