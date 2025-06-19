#include <iostream>
#include <vector>

#include "genesis.h"

blockchain::Genesis::Genesis(){
    this->previousHash = "genesis";
    this->timestamp = "";
}