#pragma once
#include <iostream>

namespace blockchain
{
    class TranstractionStatus
    {
    public:
        static const std::string UNCONFIRMED;
        static const std::string CONFIRMED;
    };
}