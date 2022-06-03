#pragma once

#include <pch.h>
#include <QVector>
#include <QCryptographicHash>
#include <cryptography.h>
class Blockchain
{
public:
    class Transaction
    {
    public:
        Transaction(const std::string& information, const CryptoPP::RSA::PrivateKey& provateKey);
        std::string getSignature()const;
        std::string getInformation()const;
        QDateTime getTime()const;
        ~Transaction() = default;
    private:
        std::string m_information;
        std::string m_digitalSignature;
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
        const QVector<Transaction>& getTransactions()const;
        bool addTransaction(const Transaction& transaction);
        bool addTransaction(Transaction&& transaction);
        bool isCompleted()const;
        void complete();
        ~Block() = default;

    private:
        void calculateBlockHash();


    private:
        QVector<Transaction> m_transactions;
        QString m_prevBlockHash;
        QString m_address;
        QString m_blockHash;
        QDateTime m_time;
        size_t m_blockNumber;
        bool m_completed;
    };

    size_t getBlocksCount()const;
    void addBlock(const Block& newBlock);
    void addBlock(Block&& newBlock);
    const QVector<Block>& getBlockChain()const;
    QString getLastBlockHash()const;

private:
    QVector<Block> m_blocks;

    //QString SignData(const QString& data, )
};
