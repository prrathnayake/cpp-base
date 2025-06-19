#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include "miner.h"
#include "../block/block.h"
#include "../memPool.h"

void blockchain::Miner::onTranstractionsFromMempool()
{
    // TODO : fetch Transtraction from memPool
}

bool blockchain::Miner::isValidateSignature(blockchain::Transtraction transtraction)
{
    // Load the public key from string
    RSA *rsa = RSA_new();
    BIO *bio = BIO_new_mem_buf(transtraction.from.c_str(), -1);
    rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, nullptr, nullptr);
    BIO_free(bio);
    std::string data = transtraction.from + transtraction.to + std::to_string(transtraction.amount);

    // Compute the SHA-256 hash of the message
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

    // Verify the signature
    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, reinterpret_cast<const unsigned char *>(transtraction.signature.c_str()), transtraction.signature.size(), rsa);
    RSA_free(rsa);
    return result == 1;
}

std::string blockchain::Miner::mine(blockchain::Block block)
{
    std::cout << "mining.......\n";
    std::string hash = getHash(block.toString());

    bool loop = true;
    while (loop)
    {
        if (hash.rfind("0000", 0) == 0)
        {
            loop = false;
            // blocks.push_back(block);
            return hash;
        }
        else
        {
            block.setNonce(block.getNonce() + 1);
            hash = getHash(block.toString());
        }
    }
}

std::string blockchain::Miner::getHash(std::string data)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

    // Convert the hash to a hexadecimal string
    std::string hex_hash;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        char hex_byte[3];
        std::sprintf(hex_byte, "%02x", hash[i]);
        hex_hash += hex_byte;
    }
    // Print the hash
    return hex_hash;
}

void blockchain::Miner::sendBlockToBlockchain(blockchain::Block block)
{
    // TODO : broadcast validated block
}
