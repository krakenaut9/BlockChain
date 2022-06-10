#pragma once

#include <pch.h>
#include <QVector>
#include <QCryptographicHash>
#include <cryptography.h>
#include <sstream>
class Blockchain
{
public:
    class Transaction
    {
    public:
        struct Properties
        {
            static const std::string information;
            static const std::string digitalSignature;
            static const std::string time;
            static const std::string number;
        };
    public:
        Transaction(const std::string& information, const CryptoPP::RSA::PrivateKey& privateKey);
        Transaction(const std::string& information, const std::string& signature, const std::string& date, const size_t number);
        std::string getSignature()const;
        std::string getInformation()const;
        std::string getTime()const;
        void setTime(const std::string&);
        void setNumber(const size_t number);
        size_t getNumber()const;
        ~Transaction() = default;
    private:
        std::string m_information;
        std::string m_digitalSignature;
        std::string m_time;
        size_t m_number;
    };

    class Block
    {
    public:
        struct Properties
        {
            static const std::string number;
            static const std::string address;
            static const std::string transactions;
            static const std::string prevBlockAddress;
            static const std::string hash;
            static const std::string time;
        };

    public :
        Block(const size_t blockNumber, const std::string& prevBlockHash, const CryptoPP::RSA::PublicKey& address);
        CryptoPP::RSA::PublicKey getAddress()const;
        CryptoPP::RSA::PrivateKey getPrivateKey()const;
        void setPrivateKey(const CryptoPP::RSA::PrivateKey&);
        std::string getBlockHash()const;
        std::string getPrevBlockHash()const;
        std::string getTime()const;
        void setTime(const std::string&);
        size_t getTransactionsCount()const;
        size_t getBlockNumber()const;
        const QVector<Transaction>& getTransactions()const;
        bool addTransaction(const Transaction& transaction);
        bool addTransaction(Transaction&& transaction);
        bool isAuthorized()const;
        void authorize();
        ~Block() = default;

    private:
        void calculateBlockHash();


    private:
        CryptoPP::RSA::PublicKey m_address;
        CryptoPP::RSA::PrivateKey m_privateKey;
        std::string m_prevBlockHash;
        std::string m_blockHash;
        std::string m_time;
        QVector<Transaction> m_transactions;
        size_t m_blockNumber;
        bool m_authorized;
    };

    size_t getBlocksCount()const;
    Block& getBlockById(const size_t blockId);
    void addBlock(const Block& newBlock);
    void addBlock(Block&& newBlock);
    QVector<Block>& getBlockChain();
    Block& getLastBlock();
    CryptoPP::RSA::PublicKey getLastBlockHash()const;

private:
    QVector<Block> m_blocks;
};
