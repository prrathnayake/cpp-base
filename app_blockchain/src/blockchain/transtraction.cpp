#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>

#include "transtraction.h"

blockchain::Transtraction::Transtraction()
{
}

blockchain::Transtraction::Transtraction(std::string p, std::string c, std::string k, std::string v)
{
    PublicKey = c;
    key = k;
    value = v;
    if (p != "privateKey")
    {
        signature = generateSignature(p);
    }
}

std::string blockchain::Transtraction::toString()
{
    std::string str = "ChangeBy : " + PublicKey + "\n" + "Key : " + key + "\n" + "Value : " + value + "\n" + "Signature : " + "\n" + signature + "\n";
    return str;
}

std::string blockchain::Transtraction::getKey()
{
    return key;
}

std::string blockchain::Transtraction::getValue()
{
    return value;
}

std::string blockchain::Transtraction::generateSignature(std::string privateKey)
{
    // Load the private key from string
    RSA *rsa = RSA_new();
    BIO *bio = BIO_new_mem_buf(privateKey.c_str(), -1);
    rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, nullptr, nullptr);
    BIO_free(bio);

    // Compute the SHA-256 hash of the message
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string data = PublicKey + key + value;
    SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

    // Create the signature
    unsigned char signature_buf[RSA_size(rsa)];
    unsigned int signature_len;
    if (!RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, signature_buf, &signature_len, rsa))
    {
        std::cerr << "Error: could not create signature" << std::endl;
        RSA_free(rsa);
    }
    // Convert the signature to a string
    std::string signature = std::string(reinterpret_cast<const char *>(signature_buf), signature_len);
    RSA_free(rsa);

    return signature;
}

bool blockchain::Transtraction::isValidateSignature(){
    // Load the public key from string
    RSA* rsa = RSA_new();
    BIO* bio = BIO_new_mem_buf(PublicKey.c_str(), -1);
    rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, nullptr, nullptr);
    BIO_free(bio);

    // Compute the SHA-256 hash of the message
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string data = PublicKey + key + value;
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    // Verify the signature
    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, reinterpret_cast<const unsigned char*>(signature.c_str()), signature.size(), rsa);
    RSA_free(rsa);
    return result == 1;
}