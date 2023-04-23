#include <iostream>

#include "block.h"

blockchain::Block::Block(std::string previousHash, blockchain::Transtraction transtraction, std::string timestamp)
{
    preHash = previousHash;
    trans = transtraction;
    ts = timestamp;
}

std::string blockchain::Block::toString()
{
    std::string str = "Nonce : " + std::to_string(getNonce()) + "\n" + "Pre Hash : " + preHash + "\n" + "Timestamp : " + ts + "\n" + "[Transtraction]" + "\n";
    return str;
}

std::string blockchain::Block::toStringWithTranstraction()
{
    std::string str = "Nonce : " + std::to_string(getNonce()) + "\n" + "Pre Hash : " + preHash + "\n" + "Timestamp : " + ts + "\n" + "[Transtraction]" + "\n" + trans.toString() + "\n";
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