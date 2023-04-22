#pragma once
#include <iostream>
#include <map>

namespace blockchain
{
    class User
    {
    private:
        std::string name;
        std::string privateKey;
        std::string publicKey;

    public:
        User(std::string name);
        std::map<std::string, std::string> generateKeyPair();
        std::string getPublicKey();
        std::string getPrivateKey();
    };
}