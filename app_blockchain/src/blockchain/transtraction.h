#pragma once
#include <iostream>
#include <map>

namespace blockchain
{
    class Transtraction
    {
    private:
        std::string PublicKey;
        std::string timestamp;
        std::string key;
        std::string value;
        std::string signature;

    public:
        Transtraction();
        Transtraction(std::string privateKey,std::string publicKey, std::string key, std::string value);
        std::string toString();
        std::string getKey();
        std::string getValue();
        std::string generateSignature(std::string privateKey);
        bool isValidateSignature();
    };
}