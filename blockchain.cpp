#include "blockchain.h"

Blockchain::Block::Block(const size_t blockNumber, const QString& prevBlockHash, const QString& address) :
    m_prevBlockHash(prevBlockHash), m_address(address), m_time(QDateTime::currentDateTime()), m_blockNumber(blockNumber), m_completed(false)
{
    qDebug() << "Create block : number = " << m_blockNumber << "address = " << m_address << " prevBlockHash = " << m_prevBlockHash;
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

bool Blockchain::Block::addTransaction(const Transaction& transaction)
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
    qDebug() << "Calculating block hash : \nprevBlockHash = " << m_prevBlockHash;
    for(size_t i = 0; i < m_transactions.size(); ++i)
    {
        qDebug() << i+1 << " transaction information = " <<m_transactions[i].getInformation();
    }
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
