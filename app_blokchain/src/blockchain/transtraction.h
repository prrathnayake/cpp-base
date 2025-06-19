#pragma once
#include <iostream>
#include <map>

#include "utils/params.h"

namespace blockchain
{
    class Transtraction
    {
    public:
        int TxID;
        std::string from;
        std::string to;
        std::string timestamp;
        float amount;
        std::string signature;
        std::string status = TranstractionStatus::UNCONFIRMED;
        
        Transtraction();
        Transtraction(std::string from, std::string to, float amount, std::string signature);
        std::string toString();
        void addNewTranstraction(std::string transtraction);
    };
}