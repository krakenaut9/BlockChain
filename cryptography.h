#pragma once

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

namespace Cryptography
{
    std::string SignData(_In_ const std::string& Data, _In_ const CryptoPP::RSA::PrivateKey& privateKey);
    void CheckSignature(_In_ const std::string& Data, _In_ const std::string& signature, _In_ const CryptoPP::RSA::PublicKey& publicKey);
}