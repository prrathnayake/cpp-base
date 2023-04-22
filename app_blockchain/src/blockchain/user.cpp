#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "user.h"

blockchain::User::User(std::string userName)
{
    std::map<std::string, std::string> pair = generateKeyPair();

    name = userName;
    privateKey = pair["privateKey"];
    publicKey = pair["publicKey"];
}

std::map<std::string, std::string> blockchain::User::generateKeyPair()
{
    std::map<std::string, std::string> pair;

    // Generate a 2048-bit RSA key pair
    RSA *rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);

    // Write the private key to a memory buffer
    BIO *priv_bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(priv_bio, rsa, nullptr, nullptr, 0, nullptr, nullptr);

    // Read the private key from the memory buffer
    char *priv_key;
    long priv_key_len = BIO_get_mem_data(priv_bio, &priv_key);
    std::string private_key(priv_key, priv_key_len);

    // Write the public key to a memory buffer
    BIO *pub_bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pub_bio, rsa);

    // Read the public key from the memory buffer
    char *pub_key;
    long pub_key_len = BIO_get_mem_data(pub_bio, &pub_key);
    std::string public_key(pub_key, pub_key_len);

    // Free memory
    RSA_free(rsa);
    BIO_free_all(priv_bio);
    BIO_free_all(pub_bio);

    pair["privateKey"] = private_key;
    pair["publicKey"] = public_key;

    return pair;
}

std::string blockchain::User::getPublicKey(){
    return publicKey;
}

std::string blockchain::User::getPrivateKey(){
    return privateKey;
}
