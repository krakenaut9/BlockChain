#include "blockchainfile.h"

bool BlockchainFile::AddBlock(const Blockchain::Block& newBlock)
{
//    QFile file(BLOCKCHAIN_FILE_NAME);
//    file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly);
//    QJsonParseError JsonParseError;
//    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
//    file.close();
//    QJsonObject RootObject = JsonDocument.object();
//    if(RootObject.find(userName) != RootObject.constEnd())
//    {
//        qDebug() << "This user already exists";
//        QMessageBox::warning(nullptr, "User exists", "This user already exists");
//        return false;
//    }
//    RootObject.insert(newUserName, userProperties);
//    JsonDocument.setObject(RootObject); // set to json document
//    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
//    file.write(JsonDocument.toJson());
//    file.close();
      return true;
}


void BlockchainFile::Load(const std::string& filename, CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSource file(filename.c_str(), true /*pumpAll*/);
    file.TransferTo(bt);
    bt.MessageEnd();
}


void BlockchainFile::LoadPublicKey(const std::string& filename, CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    Load(filename, queue);

    key.Load(queue);
}

void BlockchainFile::LoadPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    Load(filename, queue);

    key.Load(queue);
}

void BlockchainFile::Save(const std::string& filename, const CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSink file(filename.c_str());

    bt.CopyTo(file);
    file.MessageEnd();
}

void BlockchainFile::SavePublicKey(const std::string& filename, const CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    Save(filename, queue);
}

void BlockchainFile::SavePrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    Save(filename, queue);
}

void BlockchainFile::SaveHexPrivateKey(const std::string& filename, const CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    SaveHex(filename, queue);
}

void BlockchainFile::SaveHexPublicKey(const std::string& filename, const CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    key.Save(queue);

    SaveHex(filename, queue);
}

void BlockchainFile::SaveHex(const std::string& filename, const CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::HexEncoder encoder;

    bt.CopyTo(encoder);
    encoder.MessageEnd();

    Save(filename, encoder);
}

void BlockchainFile::LoadHex(const std::string& filename, CryptoPP::BufferedTransformation& bt)
{
    CryptoPP::FileSource file(filename.c_str(), true /*pumpAll*/);
    CryptoPP::HexDecoder decoder;
    file.TransferTo(decoder);
    decoder.MessageEnd();
    decoder.TransferTo(bt);
    bt.MessageEnd();
}


void BlockchainFile::LoadHexPublicKey(const std::string& filename, CryptoPP::PublicKey& key)
{
    CryptoPP::ByteQueue queue;
    LoadHex(filename, queue);

    key.Load(queue);
}

void BlockchainFile::LoadHexPrivateKey(const std::string& filename, CryptoPP::PrivateKey& key)
{
    CryptoPP::ByteQueue queue;
    LoadHex(filename, queue);

    key.Load(queue);
}

void BlockchainFile::RSAPublicKeyToHex(const CryptoPP::RSA::PublicKey& publicKey, std::string& outHexPublicKey)
{
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(outHexPublicKey));
    CryptoPP::ByteQueue queue;
    publicKey.Save(queue);
    queue.CopyTo(encoder);
    encoder.MessageEnd();
}

void BlockchainFile::RSAPrivateKeyToHex(const CryptoPP::RSA::PrivateKey& privateKey, std::string& outHexPrivateKey)
{
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(outHexPrivateKey));
    CryptoPP::ByteQueue queue;
    privateKey.Save(queue);
    queue.CopyTo(encoder);
    encoder.MessageEnd();
}
