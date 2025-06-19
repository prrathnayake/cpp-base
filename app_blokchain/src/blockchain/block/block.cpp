#include <iostream>

#include "block.h"

blockchain::Block::Block()
{
}

blockchain::Block::Block(std::string previousHash, std::vector<blockchain::Transtraction> transtractions, std::string timestamp)
{
    this->previousHash = previousHash;
    this->transtractions = transtractions;
    this->timestamp = timestamp;
}

std::string blockchain::Block::toString()
{
    std::string str = "Nonce : " + std::to_string(getNonce()) + "\n" + "Pre Hash : " + previousHash + "\n" + "Timestamp : " + timestamp + "\n" + "[Transtraction]" + "\n";
    return str;
}

int blockchain::Block::getNonce()
{
    return nonce;
}

void blockchain::Block::setNonce(int n)
{
    nonce = n;
}