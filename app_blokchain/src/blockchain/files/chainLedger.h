#pragma once
#include <iostream>
#include <fstream>

#include "ledger.h"

namespace blockchain
{
    class ChainLedger : public Ledger
    {
    public:
       std::fstream chainLedger;
    };
}