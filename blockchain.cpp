#include "blockchain.h"

Blockchain::Block::Block(const size_t blockNumber, const QString& prevBlockHash, const QString& address) :
    m_prevBlockHash(prevBlockHash), m_address(address), m_time(QDateTime::currentDateTime()), m_blockNumber(blockNumber)
{
    qDebug() << "Create block : address = " << m_address << " prevBlockHash = " << m_prevBlockHash;
}

QString Blockchain::Block::getAddress()const
{
    return m_address;
}

QString Blockchain::Block::getBlockHash()const
{
    return m_blockHash;
}

QDateTime Blockchain::Block::getTime()const
{
    return m_time;
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

void Blockchain::Block::addTransaction(const Transaction& transaction)
{
    m_transactions.push_back(transaction);
}

void Blockchain::Block::addTransaction(Transaction&& transaction)
{
    m_transactions.push_back(transaction);
}


Blockchain::Transaction::Transaction(const QString& information) : m_information(information), m_time(QDateTime::currentDateTime())
{
    qDebug() << "Transaction. Information = " << m_information;
}

QString Blockchain::Transaction::getSignature()const
{
    return m_digitalSignature;
}

QString Blockchain::Transaction::getInformation()const
{
    return m_information;
}

QDateTime Blockchain::Transaction::getTime()const
{
    return m_time;
}
