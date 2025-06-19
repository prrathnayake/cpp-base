#pragma once
#include <iostream>
#include <map>

namespace blockchain
{
    class CryptoRSA
    {
    public:
        static std::map<std::string, std::string> generateKeyPair();
        static std::string generateSignature(std::string private_key, std::string message);
        static bool validateSignature(std::string public_key, std::string message, std::string signature);
    };
}