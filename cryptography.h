#pragma once

#include <cryptopp/files.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

namespace Cryptography
{
    std::string SignData(_In_ const std::string& Data, _In_ const CryptoPP::RSA::PrivateKey& privateKey);

    void CheckSignature(_In_ const std::string& Data, _In_ const std::string& signature, _In_ const CryptoPP::RSA::PublicKey& publicKey);

    std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey> GenerateRSAKeyPair();

    void Load(const std::string& filename, CryptoPP::BufferedTransformation& bt);

    void LoadPublicKey(const std::string& filename, CryptoPP::PublicKey& key);

    void LoadPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key);

    void Save(const std::string& filename, const CryptoPP::BufferedTransformation& bt);

    void SavePublicKey(const std::string& filename, const CryptoPP::PublicKey& key);

    void SavePrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key);
}
