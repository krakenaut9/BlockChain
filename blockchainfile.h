#pragma once
#include <blockchain.h>
#include <cryptopp/hex.h>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace BlockchainFile
{
    constexpr char BLOCKCHAIN_FILE_NAME[] = "Blockchain.json";
    bool AddBlock(const Blockchain::Block& newBlock);

    void LoadPublicKey(const std::string& filename, CryptoPP::PublicKey& key);

    void LoadPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key);

    void Save(const std::string& filename, const CryptoPP::BufferedTransformation& bt);

    void SavePublicKey(const std::string& filename, const CryptoPP::PublicKey& key);

    void SavePrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key);

    void SaveHexPrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key);

    void SaveHexPublicKey(const std::string& filename, const CryptoPP::PublicKey& key);

    void SaveHex(const std::string& filename, const CryptoPP::BufferedTransformation& bt);

    void Load(const std::string& filename, CryptoPP::BufferedTransformation& bt);

    void LoadHex(const std::string& filename, CryptoPP::BufferedTransformation& bt);

    void LoadHexPublicKey(const std::string& filename, CryptoPP::PublicKey& key);

    void LoadHexPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key);

    void RSAPublicKeyToHex(const CryptoPP::RSA::PublicKey& publicKey, std::string& outHexPublicKey);

    void RSAPrivateKeyToHex(const CryptoPP::RSA::PrivateKey& privateKey, std::string& outHexPrivateKey);
}
