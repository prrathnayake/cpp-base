#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "node.h"
#include "../transtraction.h"
#include "../crypto/rsa.h"

blockchain::Node::Node()
{
    std::map<std::string, std::string> pair = blockchain::CryptoRSA::generateKeyPair();

    privateKey = pair["privateKey"];
    publicKey = pair["publicKey"];
}

void blockchain::Node::createTranstraction(std::string to, float amount)
{
    blockchain::Transtraction transtraction(publicKey, to, amount, blockchain::CryptoRSA::generateSignature(privateKey, (publicKey + to + std::to_string(amount))));
    // TODO :: broadcast transtraction
}
