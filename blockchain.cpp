#include "blockchain.h"

const std::string Blockchain::Block::Properties::number = "Number";
const std::string Blockchain::Block::Properties::address = "Address";
const std::string Blockchain::Block::Properties::transactions = "Transactions";
const std::string Blockchain::Block::Properties::prevBlockAddress = "Previous Block Hash";
const std::string Blockchain::Block::Properties::hash = "Block hash";
const std::string Blockchain::Block::Properties::time = "Creation Time";

const std::string Blockchain::Transaction::Properties::information = "Information";
const std::string Blockchain::Transaction::Properties::digitalSignature = "Digital Signature";
const std::string Blockchain::Transaction::Properties::time = "Time";
const std::string Blockchain::Transaction::Properties::number = "Number";

size_t Blockchain::getBlocksCount()const
{
    return m_blocks.size();
}

void Blockchain::addBlock(const Block& newBlock)
{
    m_blocks.push_back(newBlock);
}

void Blockchain::addBlock(Block&& newBlock)
{
    //TODO : Add block number check and throw exception
    m_blocks.push_back(newBlock);
}

QVector<Blockchain::Block>& Blockchain::getBlockChain()
{
    return m_blocks;
}

Blockchain::Block& Blockchain::getBlockById(const size_t blockId)
{
    return m_blocks[blockId];
}

Blockchain::Block& Blockchain::getLastBlock()
{
    if(m_blocks.isEmpty())
    {
        throw std::runtime_error("Blockchain is empty. Can't get last block");
    }
    return m_blocks.last();
}

CryptoPP::RSA::PublicKey Blockchain::getLastBlockHash()const
{
    return m_blocks.last().getAddress();
}

Blockchain::Block::Block(const size_t blockNumber, const std::string& prevBlockHash, const CryptoPP::RSA::PublicKey& address) :
     m_address(address), m_prevBlockHash(prevBlockHash), m_time(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString()), m_blockNumber(blockNumber), m_authorized(false)
{
    QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Sha512);
    hasher->addData(m_prevBlockHash.c_str(), m_prevBlockHash.length());
    m_blockHash = hasher->result().toHex().toStdString();
}

CryptoPP::RSA::PublicKey Blockchain::Block::getAddress()const
{
    return m_address;
}

CryptoPP::RSA::PrivateKey Blockchain::Block::getPrivateKey()const
{
    return m_privateKey;
}

void Blockchain::Block::setPrivateKey(const CryptoPP::RSA::PrivateKey& privateKey)
{
    m_privateKey = privateKey;
}

std::string Blockchain::Block::getBlockHash()const
{
    return m_blockHash;
}

std::string Blockchain::Block::getPrevBlockHash()const
{
    return m_prevBlockHash;
}

std::string Blockchain::Block::getTime()const
{
    return m_time;
}

void Blockchain::Block::setTime(const std::string& newTime)
{
    m_time = newTime;
}

bool Blockchain::Block::isAuthorized()const
{
    return m_authorized;
}

size_t Blockchain::Block::getTransactionsCount()const
{
    return m_transactions.size();
}

size_t Blockchain::Block::getBlockNumber()const
{
    return m_blockNumber;
}

bool Blockchain::Block::addTransaction(const Transaction& transaction)
{
    m_transactions.push_back(transaction);
    m_transactions.last().setNumber(m_transactions.size() - 1);
    QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Sha512);
    hasher->addData(m_blockHash.c_str(), m_blockHash.length());
    hasher->addData(transaction.getInformation().c_str(), transaction.getInformation().length());

    m_blockHash = hasher->result().toHex().toStdString();
    return true;
}

bool Blockchain::Block::addTransaction(Transaction&& transaction)
{
    m_transactions.push_back(transaction);
    m_transactions.last().setNumber(m_transactions.size() - 1);
    QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Sha512);
    hasher->addData(m_blockHash.c_str(), m_blockHash.length());
    hasher->addData(transaction.getInformation().c_str(), transaction.getInformation().length());

    m_blockHash = hasher->result().toHex().toStdString();
    return true;
}

void Blockchain::Block::calculateBlockHash()
{
    QCryptographicHash hash(QCryptographicHash::Sha512);
    hash.addData(m_prevBlockHash.c_str(), m_prevBlockHash.length());
    qDebug() << "Calculating block hash : \nprevBlockHash = " << m_prevBlockHash.c_str();
    for(size_t i = 0; i < m_transactions.size(); ++i)
    {
        hash.addData(m_transactions[i].getInformation().c_str(), m_transactions[i].getInformation().length());
        qDebug() << i+1 << " transaction information = " <<m_transactions[i].getInformation().c_str();
    }
    m_blockHash = hash.result().toStdString();
}

void Blockchain::Block::authorize()
{
    m_authorized = true;
    qDebug() << m_blockNumber << " block authorized";
    //calculateBlockHash();
}

const QVector<Blockchain::Transaction>& Blockchain::Block::getTransactions()const
{
    return m_transactions;
}


Blockchain::Transaction::Transaction(const std::string& information, const CryptoPP::RSA::PrivateKey& privateKey) :
    m_information(information), m_time(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString())
{
    qDebug() << "Transaction. Information = " << m_information.c_str();
    m_digitalSignature = Cryptography::SignData(information, privateKey);
    qDebug() << "Signature = " << QByteArray(m_digitalSignature.c_str(), m_digitalSignature.size()).toHex();
}

Blockchain::Transaction::Transaction(const std::string& information, const std::string& signature, const std::string& time, const size_t number):
    m_information(information), m_digitalSignature(signature), m_time(time), m_number(number)
{
    std::cout << "Read transaction : \nInformation : " << m_information << "\nsignature : " << m_digitalSignature << "\nnumber : " << m_number << "\n";
}

std::string Blockchain::Transaction::getSignature()const
{
    return m_digitalSignature;
}

std::string Blockchain::Transaction::getInformation()const
{
    return m_information;
}

std::string Blockchain::Transaction::getTime()const
{
    return m_time;
}

size_t Blockchain::Transaction::getNumber() const
{
    return m_number;
}

void Blockchain::Transaction::setTime(const std::string& newTime)
{
    m_time = newTime;
}

void Blockchain::Transaction::setNumber(const size_t number)
{
    m_number = number;
}
