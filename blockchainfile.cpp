#include "blockchainfile.h"

#include <blockchain.h>

bool BlockchainFile::AddBlock(const Blockchain::Block& newBlock)
{
    QFile file(BLOCKCHAIN_FILE_NAME);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File open failed. Can not add a new block";
        return false;
    }
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();

    if(RootObject.find(QString::number(newBlock.getBlockNumber())) != RootObject.constEnd())
    {
        qDebug() << "This block already exists";
        QMessageBox::warning(nullptr, "Block exists", "This block already exists");
        return false;
    }

    QJsonObject blockProperties;

    //Block Address
    std::string hexAddress;
    BlockchainFile::RSAPublicKeyToHex(newBlock.getAddress(), hexAddress);
    blockProperties.insert(Blockchain::Block::Properties::address.c_str(),
                           QString::fromStdString(hexAddress));


    //Transactions
    QJsonObject transactionsObject;
    for(const auto& transaction : newBlock.getTransactions())
    {
        QJsonObject transactionObject;
        transactionObject.insert(Blockchain::Transaction::Properties::information.c_str(),
                                 QString::fromStdString(transaction.getInformation()));

        transactionObject.insert(Blockchain::Transaction::Properties::digitalSignature.c_str(),
                                 QString::fromStdString(transaction.getSignature()));

        transactionObject.insert(Blockchain::Transaction::Properties::time.c_str(),
                                 QString::fromStdString(transaction.getTime()));

        transactionsObject.insert(QString::number(transaction.getNumber()), transactionObject);
    }

    blockProperties.insert(Blockchain::Block::Properties::transactions.c_str(),
                           transactionsObject);

    //Previous Block hash
    blockProperties.insert(Blockchain::Block::Properties::prevBlockAddress.c_str(),
                           QString::fromStdString(newBlock.getPrevBlockHash()));

    //Creation Time
    blockProperties.insert(Blockchain::Block::Properties::time.c_str(),
                           QString::fromStdString(newBlock.getTime()));

    //Block hash
    blockProperties.insert(Blockchain::Block::Properties::hash.c_str(),
                           QString::fromStdString(newBlock.getBlockHash()));

    RootObject.insert(QString::number(newBlock.getBlockNumber())
                      , blockProperties);
    JsonDocument.setObject(RootObject); // set to json document
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(JsonDocument.toJson());
    file.close();
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

void BlockchainFile::HexToRSAPublicKey(const std::string& HexPublicKey, CryptoPP::RSA::PublicKey& publicKey)
{
    CryptoPP::HexDecoder decoder;
    CryptoPP::StringSource ss(HexPublicKey, true);
    ss.CopyTo(decoder);
    decoder.MessageEnd();
    CryptoPP::ByteQueue queue;
    decoder.TransferTo(queue);
    publicKey.Load(queue);
}

void BlockchainFile::HexToRSAPrivateKey(const std::string& HexPrivateKey, CryptoPP::RSA::PrivateKey& privateKey)
{
    CryptoPP::HexDecoder decoder;
    CryptoPP::StringSource ss(HexPrivateKey, true);
    ss.CopyTo(decoder);
    decoder.MessageEnd();
    CryptoPP::ByteQueue queue;
    decoder.TransferTo(queue);
    privateKey.Load(queue);
}

void BlockchainFile::ReadBlockchainFromFile(Blockchain& blockchain, const char filename[])
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        qDebug() << "File open failed for initializing the blockchain : " << filename;
        file.open(QIODevice::NewOnly | QIODevice::ReadWrite | QIODevice::Text);
        file.close();
        return;
    }
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    for(auto it = RootObject.constBegin(); it != RootObject.constEnd(); ++it)
    {
        auto blockNumber = it.key().toInt();
        auto prevBlockHash = it.value().toObject()[Blockchain::Block::Properties::prevBlockAddress.c_str()].toString();
        auto addressHex = it.value().toObject()[Blockchain::Block::Properties::address.c_str()].toString();
        CryptoPP::RSA::PublicKey address;
        HexToRSAPublicKey(addressHex.toStdString(), address);
        Blockchain::Block block(blockNumber, prevBlockHash.toStdString(), address);
        block.setTime(it.value().toObject()[Blockchain::Block::Properties::time.c_str()].toString().toStdString());
        blockchain.addBlock(block);
        qDebug() << "Read block number = " << blockNumber << " prevhash = " << prevBlockHash << " address " << addressHex;
    }

}
