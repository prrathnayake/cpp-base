#include <iostream>

#include "block.h"

blockchain::Block::Block(std::string previousHash, blockchain::Transtraction transtraction, std::string timestamp)
{
    preHash = previousHash;
    trans = transtraction;
    ts = timestamp;
}

void blockchain::Block::generateHash() {}

std::string blockchain::Block::toString()
{
    std::string str = "Nonce : " + std::to_string(nonce) + "\n" + "Pre Hash : " + preHash + "\n" + "Timestamp : " + ts + "\n" + "[Transtraction]" + "\n";
    return str;
}

std::string blockchain::Block::toStringWithTranstraction()
{
    std::string str = "Nonce : " + std::to_string(nonce) + "\n" + "Pre Hash : " + preHash + "\n" + "Timestamp : " + ts + "\n" + "[Transtraction]" + "\n" + trans.toString() + "\n";
    return str;
}