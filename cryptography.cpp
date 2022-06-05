#include "cryptography.h"

std::string Cryptography::SignData(_In_ const std::string& Data, _In_ const CryptoPP::RSA::PrivateKey& privateKey)
{
    using namespace CryptoPP;
    RSASS<PKCS1v15,SHA512>::Signer signer(privateKey);

    std::string signature;
    AutoSeededRandomPool rng;
    StringSource ss(
                Data,
                true,
                new SignerFilter(
                    rng, signer,
                    new StringSink(signature)
                    ) // SignerFilter
                ); // StringSource

    return signature;
}

void Cryptography::CheckSignature(_In_ const std::string& data, _In_ const std::string& signature, _In_ const CryptoPP::RSA::PublicKey& publicKey)
{
    using namespace CryptoPP;
    RSASS<PKCS1v15,SHA512>::Verifier verifier(publicKey);
    StringSource ss(data+signature, true,
        new SignatureVerificationFilter(
            verifier, NULL,
            SignatureVerificationFilter::THROW_EXCEPTION
       ) // SignatureVerificationFilter
    ); // StringSource
}

std::pair<CryptoPP::RSA::PublicKey, CryptoPP::RSA::PrivateKey> Cryptography::GenerateRSAKeyPair()
{
    using namespace CryptoPP;
    AutoSeededRandomPool rng;
    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 2048);
    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);

    return std::make_pair(publicKey, privateKey);
}

void Cryptography::Load(const std::string& filename, CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSource file(filename.c_str(), true /*pumpAll*/);
    file.TransferTo(bt);
    bt.MessageEnd();
}


void Cryptography::LoadPublicKey(const std::string& filename, CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    Load(filename, queue);

    key.Load(queue);
}

void Cryptography::LoadPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    Load(filename, queue);

    key.Load(queue);
}

void Cryptography::Save(const std::string& filename, const CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSink file(filename.c_str());

    bt.CopyTo(file);
    file.MessageEnd();
}

void Cryptography::SavePublicKey(const std::string& filename, const CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    Save(filename, queue);
}

void Cryptography::SavePrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    Save(filename, queue);
}
