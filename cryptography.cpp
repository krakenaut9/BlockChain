#include "cryptography.h"

std::string Cryptography::SignData(_In_ const std::string& Data, _In_ const CryptoPP::RSA::PrivateKey& privateKey)
{
    using namespace CryptoPP;
    RSASS<PKCS1v15,SHA512>::Signer signer(privateKey);

    std::string signature;
    AutoSeededRandomPool rng;
    StringSink ss(signature);
    SignerFilter sf(rng, signer, &ss);
    StringSource ss1(
                Data,
                true,
                &sf // SignerFilter
                ); // StringSource

    return signature;
}

void Cryptography::CheckSignature(_In_ const std::string& data, _In_ const std::string& signature, _In_ const CryptoPP::RSA::PublicKey& publicKey)
{
    using namespace CryptoPP;
    RSASS<PKCS1v15,SHA512>::Verifier verifier(publicKey);
    SignatureVerificationFilter SVFilter(
                verifier,
                nullptr,
                SignatureVerificationFilter::THROW_EXCEPTION
                );
    StringSource ss2(data+signature, true, &SVFilter);
}
