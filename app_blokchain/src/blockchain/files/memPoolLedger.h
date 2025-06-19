#pragma once
#include <iostream>
#include <fstream>

#include "ledger.h"

namespace blockchain
{
    class MemPoolLedger : public Ledger
    {
    public:
       std::fstream memPoolLedger;
    };
}