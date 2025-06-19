#pragma once
#include <iostream>

namespace blockchain
{
    class Coin
    {
    private:
        std::string ID;
        std::string timestamp;

    public:
        Coin();
    };
}