#pragma once

#include <pch.h>
#include <QVector>

namespace Blockchain
{
    class Transaction
    {
    public:
        Transaction(const QString& information);
        QString getSignature()const;
        QString getInformation()const;
        QDateTime getTime()const;
        ~Transaction() = default;
    private:
        QString m_information;
        QString m_digitalSignature;
        QDateTime m_time;
    };

    class Block
    {
    public :
        Block(const size_t blockNumber, const QString& prevBlockHash = "", const QString& address = "");
        QString getAddress()const;
        QString getBlockHash()const;
        QDateTime getTime()const;
        size_t getTransactionsCount()const;
        size_t getBlockNumber()const;
        void addTransaction(const Transaction& transaction);
        void addTransaction(Transaction&& transaction);
        bool isCompleted()const;
        ~Block() = default;
    private:



    private:
        QVector<Transaction> m_transactions;
        QString m_prevBlockHash;
        QString m_address;
        QString m_blockHash;
        QDateTime m_time;
        size_t m_blockNumber;
        bool m_completed;
    };
}
