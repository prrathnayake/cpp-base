#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include "transtraction.h"
#include "memPool.h"

blockchain::Transtraction::Transtraction()
{
}

blockchain::Transtraction::Transtraction(std::string from, std::string to, float amount, std::string signature)
{
    this->from = from;
    this->to = to;
    this->amount = amount;
    this->signature = signature;
}

std::string blockchain::Transtraction::toString()
{
    std::string str = "from : " + from + "\n" + "to : " + to + "\n" + "amount : " + std::to_string(amount) + "\n";
    return str;
}

void blockchain::Transtraction::addNewTranstraction(std::string transtraction)
{
}