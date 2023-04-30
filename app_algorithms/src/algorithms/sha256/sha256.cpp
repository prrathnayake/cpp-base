#include <iostream>
#include <vector>
#include <map>

#include "common.h"
#include "formulas.h"
#include "conversions.h"
#include "sha256.h"

std::string K[64] = {"0x428a2f98", "0x71374491", "0xb5c0fbcf", "0xe9b5dba5", "0x3956c25b", "0x59f111f1", "0x923f82a4", "0xab1c5ed5",
                     "0xd807aa98", "0x12835b01", "0x243185be", "0x550c7dc3", "0x72be5d74", "0x80deb1fe", "0x9bdc06a7", "0xc19bf174",
                     "0xe49b69c1", "0xefbe4786", "0x0fc19dc6", "0x240ca1cc", "0x2de92c6f", "0x4a7484aa", "0x5cb0a9dc", "0x76f988da",
                     "0x983e5152", "0xa831c66d", "0xb00327c8", "0xbf597fc7", "0xc6e00bf3", "0xd5a79147", "0x06ca6351", "0x14292967",
                     "0x27b70a85", "0x2e1b2138", "0x4d2c6dfc", "0x53380d13", "0x650a7354", "0x766a0abb", "0x81c2c92e", "0x92722c85",
                     "0xa2bfe8a1", "0xa81a664b", "0xc24b8b70", "0xc76c51a3", "0xd192e819", "0xd6990624", "0xf40e3585", "0x106aa070",
                     "0x19a4c116", "0x1e376c08", "0x2748774c", "0x34b0bcb5", "0x391c0cb3", "0x4ed8aa4a", "0x5b9cca4f", "0x682e6ff3",
                     "0x748f82ee", "0x78a5636f", "0x84c87814", "0x8cc70208", "0x90befffa", "0xa4506ceb", "0xbef9a3f7", "0xc67178f2"};

std::string H[8] = {"0x6a09e667", "0xbb67ae85", "0x3c6ef372", "0xa54ff53a", "0x510e527f", "0x9b05688c", "0x1f83d9ab", "0x5be0cd19"};

std::string padding(std::string binary)
{
    int binarySize = binary.size();
    binary += "1";
    int k = 448 - binary.size();
    for (int i = 0; i < k; i++)
    {
        binary += "0";
    }

    std::string length = decimalToBinary(binarySize);

    int len = 64 - length.size();
    for (int i = 0; i < len; i++)
    {
        binary += "0";
    }
    binary += length;

    return binary;
}

std::vector<std::vector<std::string>> blockDecomposition(std::string binary)
{
    std::vector<std::vector<std::string>> blocksAndWords;
    binary = padding(binary);
    std::vector<std::string> blocks;
    int len = binary.size();
    for (int i = 0; i < len / 512; i++)
    {
        char buffer[512];
        binary.copy(buffer, 512, 0);
        binary.erase(0, 512);
        blocks.push_back(std::string(buffer, 512));
    }

    std::vector<std::string> words;
    for (int i = 0; i < blocks.size(); i++)
    {
        std::string hash = blocks[i];
        for (int j = 0; j < blocks[i].size() / 32; j++)
        {
            char buffer[32];
            hash.copy(buffer, 32, 0);
            hash.erase(0, 32);
            words.push_back(std::string(buffer, 32));
        }

        for (int j = 16; j < 64; j++)
        {
            words.push_back(additionModulo(additionModulo(additionModulo(sigma1(words[j - 2]), words[j - 7]), (sigma0(words[j - 15]))), words[j - 16]));
        }
        blocksAndWords.push_back(words);
    }
    return blocksAndWords;
}

std::string hashComputation(std::string binary)
{
    std::vector<std::vector<std::string>> blocks = blockDecomposition(binary);
    std::string a, b, c, d, e, f, g, h, T1, T2;

    for (int i = 0; i < blocks.size(); i++)
    {
        a = hexToBinary(H[0]);
        b = hexToBinary(H[1]);
        c = hexToBinary(H[2]);
        d = hexToBinary(H[3]);
        e = hexToBinary(H[4]);
        f = hexToBinary(H[5]);
        g = hexToBinary(H[6]);
        h = hexToBinary(H[7]);
        for (int j = 0; j < blocks[i].size(); j++)
        {
            T1 = additionModulo(additionModulo(additionModulo(additionModulo(Ch(e, f, g), Sigma1(e)), h), hexToBinary(K[j])), blocks[i][j]);
            T2 = additionModulo(Sigma0(a), Maj(a, b, c));
            h = g;
            g = f;
            f = e;
            e = additionModulo(d, T1);
            d = c;
            c = b;
            b = a;
            a = additionModulo(T1, T2);
        }
        H[0] = binaryToHex(additionModulo(hexToBinary(H[0]), a));
        H[1] = binaryToHex(additionModulo(hexToBinary(H[1]), b));
        H[2] = binaryToHex(additionModulo(hexToBinary(H[2]), c));
        H[3] = binaryToHex(additionModulo(hexToBinary(H[3]), d));
        H[4] = binaryToHex(additionModulo(hexToBinary(H[4]), e));
        H[5] = binaryToHex(additionModulo(hexToBinary(H[5]), f));
        H[6] = binaryToHex(additionModulo(hexToBinary(H[6]), g));
        H[7] = binaryToHex(additionModulo(hexToBinary(H[7]), h));
    }
    return H[0] + H[1] + H[2] + H[3] + H[4] + H[5] + H[6] + H[7];
}