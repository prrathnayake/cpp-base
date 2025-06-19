#include <iostream>
#include <map>

#include "transtraction.h"
#include "memPool.h"

void blockchain::MemPool::addTranstractionToMemPool(blockchain::Transtraction transtraction){
    // TODO :: fetch trastraction
    transtractions[transtraction.TxID] = transtraction;
}

void blockchain::MemPool::removeTranstractionFromMemPool(int TxID){
    // TODO :: broacast trastractions on mempool on miner reqest
    transtractions.erase(TxID);
}