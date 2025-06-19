#pragma once
#include <iostream>
#include <fstream>

#include "ledger.h"

namespace blockchain
{
    class PeerLedger : public Ledger
    {
    public:
       std::fstream peerLedger;
    };
}