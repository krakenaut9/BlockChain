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

const QVector<Blockchain::Block>& Blockchain::getBlockChain()const
{
    return m_blocks;
}

Blockchain::Block& Blockchain::getBlockById(const size_t blockId)
{
    return m_blocks[blockId];
}

const Blockchain::Block& Blockchain::getLastBlock()const
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
    m_prevBlockHash(prevBlockHash), m_address(address), m_time(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString()), m_blockNumber(blockNumber), m_completed(false)
{
    QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Sha512);
    hasher->addData(m_prevBlockHash.c_str(), m_prevBlockHash.length());
    m_blockHash = hasher->result().toHex().toStdString();
}

CryptoPP::RSA::PublicKey Blockchain::Block::getAddress()const
{
    return m_address;
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

bool Blockchain::Block::isCompleted()const
{
    return m_completed;
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
    if(!m_completed)
    {
        m_transactions.push_back(transaction);
        QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Sha512);
        hasher->addData(m_blockHash.c_str(), m_blockHash.length());
        hasher->addData(transaction.getInformation().c_str(), transaction.getInformation().length());

        m_blockHash = hasher->result().toHex().toStdString();
        return true;
    }
    else
    {
        qDebug() << m_blockNumber << " block is full. Can't add a new transaction";
        return false;
    }
}

bool Blockchain::Block::addTransaction(Transaction&& transaction)
{
    if(!m_completed)
    {
        m_transactions.push_back(transaction);
        return true;
    }
    else
    {
        qDebug() << m_blockNumber << " block is full. Can't add a new transaction";
        return false;
    }
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

void Blockchain::Block::complete()
{
    m_completed = true;
    qDebug() << m_blockNumber << " block completed";
    calculateBlockHash();
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
