#include "mainwindow.h"
#include "ui_mainwindow.h"

Blockchain g_blockchain;



namespace
{
    static std::string messageToCheck = "Dima Maslo";
    bool VerifyRSAKeyPair(
            const CryptoPP::RSA::PublicKey& publicKey,
            const CryptoPP::RSA::PrivateKey& privateKey,
            const Blockchain::Block& block
            )
    {
        auto signature = Cryptography::SignData(
                block.getTransactions()[0].getInformation(),
                privateKey
                );

        try
        {
            Cryptography::CheckSignature(
                    block.getTransactions()[0].getInformation(),
                    signature,
                    publicKey);
        }
        catch (const std::exception& ex)
        {
            qDebug() << ex.what();
            return false;
        }
        return true;
    }

    void ManageBlockTransactions(Blockchain::Block& block)
    {
        TransactionsWindow tw(&block);
        tw.exec();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    BlockchainFile::ReadBlockchainFromFile(g_blockchain, BlockchainFile::BLOCKCHAIN_FILE_NAME);
    connect(ui->addBlockPushButton, &QPushButton::clicked, this, &MainWindow::newBlockClicked);
    connect(ui->nextBlockPushButton, &QPushButton::clicked, this, &MainWindow::nextBlockClicked);
    connect(ui->prevBlockPushButton, &QPushButton::clicked, this, &MainWindow::prevBlockClicked);
    connect(ui->gotoPushButton, &QPushButton::clicked, this, &MainWindow::gotoClicked);
    connect(ui->authorizeBlockPushButton1, &QPushButton::clicked, this, &MainWindow::activate1);
    connect(ui->authorizeBlockPushButton2, &QPushButton::clicked, this, &MainWindow::activate2);
    connect(ui->authorizeBlockPushButton3, &QPushButton::clicked, this, &MainWindow::activate3);
    connect(ui->transactionsPushButton1, &QPushButton::clicked, this, &MainWindow::showTransactions1);
    connect(ui->transactionsPushButton2, &QPushButton::clicked, this, &MainWindow::showTransactions2);
    connect(ui->transactionsPushButton3, &QPushButton::clicked, this, &MainWindow::showTransactions3);
    ui->addressLineEdit1->setReadOnly(true);
    ui->addressLineEdit2->setReadOnly(true);
    ui->addressLineEdit3->setReadOnly(true);

    ui->dateLineEdit1->setReadOnly(true);
    ui->dateLineEdit2->setReadOnly(true);
    ui->dateLineEdit3->setReadOnly(true);

    ui->hashLineEdit1->setReadOnly(true);
    ui->hashLineEdit2->setReadOnly(true);
    ui->hashLineEdit3->setReadOnly(true);

    ui->numberLineEdit1->setReadOnly(true);
    ui->numberLineEdit2->setReadOnly(true);
    ui->numberLineEdit3->setReadOnly(true);

    ui->prevBlockPushButton->setDisabled(true);
    if(g_blockchain.getBlocksCount() <= 3)
    {
        ui->nextBlockPushButton->setDisabled(true);
    }

    for(size_t i = 0; i < (g_blockchain.getBlocksCount() > 3 ? 3 : g_blockchain.getBlocksCount()); ++i)
    {
        qDebug() << "Add block on display";
        Blockchain::Block blockRef = g_blockchain.getBlockById(i);

        std::string hexAddress;
        BlockchainFile::RSAPublicKeyToHex(blockRef.getAddress(), hexAddress);
        switch (i)
        {
        case 0 :
            ui->addressLineEdit1->setText(QString::fromStdString(hexAddress));

            ui->dateLineEdit1->setText(QString::fromStdString(blockRef.getTime()));

            ui->hashLineEdit1->setText(QString::fromStdString(blockRef.getBlockHash()));

            ui->numberLineEdit1->setText(QString::number(blockRef.getBlockNumber()));
            break;
        case 1:
            ui->addressLineEdit2->setText(QString::fromStdString(hexAddress));

            ui->dateLineEdit2->setText(QString::fromStdString(blockRef.getTime()));

            ui->hashLineEdit2->setText(QString::fromStdString(blockRef.getBlockHash()));

            ui->numberLineEdit2->setText(QString::number(blockRef.getBlockNumber()));
            break;
        case 2:
            ui->addressLineEdit3->setText(QString::fromStdString(hexAddress));

            ui->dateLineEdit3->setText(QString::fromStdString(blockRef.getTime()));

            ui->hashLineEdit3->setText(QString::fromStdString(blockRef.getBlockHash()));

            ui->numberLineEdit3->setText(QString::number(blockRef.getBlockNumber()));
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::newBlockClicked()
{
    qDebug() << "Add new block";
    auto keys = Cryptography::GenerateRSAKeyPair();

    auto privateKeyPath = QFileDialog::getSaveFileName(this, "Save private key", QDir::currentPath() + '/' + QString::number(g_blockchain.getBlocksCount()) + "_blockPrivateKey");
    if(privateKeyPath.isEmpty())
    {
        qDebug() << "Empty key path. New block is not added";
        QMessageBox::warning(this, "Private key save failed", "You did not save a private key.\n So new block is not added");
        return;
    }
    Blockchain::Transaction newUserTransaction("New user", keys.second);

    std::string hexPublicKey;
    if(g_blockchain.getBlockChain().isEmpty())
    {
        hexPublicKey = "";
    }
    else
    {
        BlockchainFile::RSAPublicKeyToHex(g_blockchain.getLastBlockHash(), hexPublicKey);
    }
    Blockchain::Block newBlock(
                g_blockchain.getBlocksCount(),
                g_blockchain.getBlocksCount() == 0 ? "" : g_blockchain.getLastBlock().getBlockHash(),
                keys.first
                );

    newBlock.addTransaction(newUserTransaction);

    g_blockchain.addBlock(newBlock);

    BlockchainFile::SaveHexPrivateKey(privateKeyPath.toStdString() + ".key", keys.second);

    BlockchainFile::AddBlock(newBlock);

    if(ui->numberLineEdit3->text().isEmpty())
    {
        std::string hexAddress;
        BlockchainFile::RSAPublicKeyToHex(newBlock.getAddress(), hexAddress);
        if(ui->numberLineEdit2->text().isEmpty())
        {
            if(ui->numberLineEdit1->text().isEmpty())
            {
                ui->addressLineEdit1->setText(QString::fromStdString(hexAddress));

                ui->dateLineEdit1->setText(QString::fromStdString(newBlock.getTime()));

                ui->hashLineEdit1->setText(QString::fromStdString(newBlock.getBlockHash()));

                ui->numberLineEdit1->setText(QString::number(newBlock.getBlockNumber()));
                return;
            }

            ui->addressLineEdit2->setText(QString::fromStdString(hexAddress));

            ui->dateLineEdit2->setText(QString::fromStdString(newBlock.getTime()));

            ui->hashLineEdit2->setText(QString::fromStdString(newBlock.getBlockHash()));

            ui->numberLineEdit2->setText(QString::number(newBlock.getBlockNumber()));
            return;
        }

        ui->addressLineEdit3->setText(QString::fromStdString(hexAddress));

        ui->dateLineEdit3->setText(QString::fromStdString(newBlock.getTime()));

        ui->hashLineEdit3->setText(QString::fromStdString(newBlock.getBlockHash()));

        ui->numberLineEdit3->setText(QString::number(newBlock.getBlockNumber()));
        return;
    }

    if(g_blockchain.getBlocksCount() > 3)
    {
        ui->nextBlockPushButton->setEnabled(true);
    }
}

void MainWindow::nextBlockClicked()
{
    qDebug() << "Next block";
    if(ui->numberLineEdit3->text().toLongLong() == g_blockchain.getBlocksCount() - 2)
    {
        qDebug() << "Currently at last block";
        ui->nextBlockPushButton->setDisabled(true);
    }

    if(!ui->prevBlockPushButton->isEnabled())
    {
        ui->prevBlockPushButton->setEnabled(true);
    }

    //move 2 to 1
    ui->addressLineEdit1->setText(ui->addressLineEdit2->text());
    ui->dateLineEdit1->setText(ui->dateLineEdit2->text());
    ui->hashLineEdit1->setText(ui->hashLineEdit2->text());
    ui->numberLineEdit1->setText(ui->numberLineEdit2->text());
    if(g_blockchain.getBlockById(ui->numberLineEdit1->text().toULongLong()).isAuthorized() && ui->authorizeBlockPushButton1->isEnabled())
    {
        ui->authorizeBlockPushButton1->setDisabled(true);
        ui->authorizeBlockPushButton1->setText("Authorized");
    }
    else if(!g_blockchain.getBlockById(ui->numberLineEdit1->text().toULongLong()).isAuthorized() && !ui->authorizeBlockPushButton1->isEnabled())
    {
        ui->authorizeBlockPushButton1->setDisabled(false);
        ui->authorizeBlockPushButton1->setText("Authorize");
    }

    //move 3 to 2
    ui->addressLineEdit2->setText(ui->addressLineEdit3->text());
    ui->dateLineEdit2->setText(ui->dateLineEdit3->text());
    ui->hashLineEdit2->setText(ui->hashLineEdit3->text());
    ui->numberLineEdit2->setText(ui->numberLineEdit3->text());
    if(g_blockchain.getBlockById(ui->numberLineEdit2->text().toULongLong()).isAuthorized() && ui->authorizeBlockPushButton2->isEnabled())
    {
        ui->authorizeBlockPushButton2->setDisabled(true);
        ui->authorizeBlockPushButton2->setText("Authorized");
    }
    else if(!g_blockchain.getBlockById(ui->numberLineEdit2->text().toULongLong()).isAuthorized() && !ui->authorizeBlockPushButton2->isEnabled())
    {
        ui->authorizeBlockPushButton2->setDisabled(false);
        ui->authorizeBlockPushButton2->setText("Authorize");
    }

    //init 3

    Blockchain::Block blockRef = g_blockchain.getBlockById(ui->numberLineEdit3->text().toULongLong() + 1);

    std::string hexAddress;
    BlockchainFile::RSAPublicKeyToHex(blockRef.getAddress(), hexAddress);

    ui->addressLineEdit3->setText(QString::fromStdString(hexAddress));

    ui->dateLineEdit3->setText(QString::fromStdString(blockRef.getTime()));

    ui->hashLineEdit3->setText(QString::fromStdString(blockRef.getBlockHash()));

    ui->numberLineEdit3->setText(QString::number(blockRef.getBlockNumber()));
    if(g_blockchain.getBlockById(ui->numberLineEdit3->text().toULongLong()).isAuthorized() && ui->authorizeBlockPushButton3->isEnabled())
    {
        ui->authorizeBlockPushButton3->setDisabled(true);
        ui->authorizeBlockPushButton3->setText("Authorized");
    }
    else if(!g_blockchain.getBlockById(ui->numberLineEdit3->text().toULongLong()).isAuthorized() && !ui->authorizeBlockPushButton3->isEnabled())
    {
        ui->authorizeBlockPushButton3->setDisabled(false);
        ui->authorizeBlockPushButton3->setText("Authorize");
    }
}

void MainWindow::prevBlockClicked()
{
    qDebug() << "Prev block";
    if(ui->numberLineEdit1->text().toLongLong() == 1)
    {
        qDebug() << "Currently at first block";
        ui->prevBlockPushButton->setDisabled(true);
    }

    if(!ui->nextBlockPushButton->isEnabled())
    {
        ui->nextBlockPushButton->setEnabled(true);
    }

    //move 2 to 3
    ui->addressLineEdit3->setText(ui->addressLineEdit2->text());
    ui->dateLineEdit3->setText(ui->dateLineEdit2->text());
    ui->hashLineEdit3->setText(ui->hashLineEdit2->text());
    ui->numberLineEdit3->setText(ui->numberLineEdit2->text());
    if(g_blockchain.getBlockById(ui->numberLineEdit3->text().toULongLong()).isAuthorized() && ui->authorizeBlockPushButton3->isEnabled())
    {
        ui->authorizeBlockPushButton3->setDisabled(true);
        ui->authorizeBlockPushButton3->setText("Authorized");
    }
    else if(!g_blockchain.getBlockById(ui->numberLineEdit3->text().toULongLong()).isAuthorized() && !ui->authorizeBlockPushButton3->isEnabled())
    {
        ui->authorizeBlockPushButton3->setDisabled(false);
        ui->authorizeBlockPushButton3->setText("Authorize");
    }

    //move 1 to 2
    ui->addressLineEdit2->setText(ui->addressLineEdit1->text());
    ui->dateLineEdit2->setText(ui->dateLineEdit1->text());
    ui->hashLineEdit2->setText(ui->hashLineEdit1->text());
    ui->numberLineEdit2->setText(ui->numberLineEdit1->text());
    if(g_blockchain.getBlockById(ui->numberLineEdit2->text().toULongLong()).isAuthorized() && ui->authorizeBlockPushButton2->isEnabled())
    {
        ui->authorizeBlockPushButton2->setDisabled(true);
        ui->authorizeBlockPushButton2->setText("Authorized");
    }
    else if(!g_blockchain.getBlockById(ui->numberLineEdit3->text().toULongLong()).isAuthorized() && !ui->authorizeBlockPushButton2->isEnabled())
    {
        ui->authorizeBlockPushButton2->setDisabled(false);
        ui->authorizeBlockPushButton2->setText("Authorize");
    }

    //init 1

    Blockchain::Block blockRef = g_blockchain.getBlockById(ui->numberLineEdit1->text().toULongLong() - 1);

    std::string hexAddress;
    BlockchainFile::RSAPublicKeyToHex(blockRef.getAddress(), hexAddress);

    ui->addressLineEdit1->setText(QString::fromStdString(hexAddress));

    ui->dateLineEdit1->setText(QString::fromStdString(blockRef.getTime()));

    ui->hashLineEdit1->setText(QString::fromStdString(blockRef.getBlockHash()));

    ui->numberLineEdit1->setText(QString::number(blockRef.getBlockNumber()));

    if(g_blockchain.getBlockById(ui->numberLineEdit1->text().toULongLong()).isAuthorized() && ui->authorizeBlockPushButton1->isEnabled())
    {
        ui->authorizeBlockPushButton1->setDisabled(true);
        ui->authorizeBlockPushButton1->setText("Authorized");
    }
    else if(!g_blockchain.getBlockById(ui->numberLineEdit1->text().toULongLong()).isAuthorized() && !ui->authorizeBlockPushButton1->isEnabled())
    {
        ui->authorizeBlockPushButton1->setDisabled(false);
        ui->authorizeBlockPushButton1->setText("Authorize");
    }
}

void MainWindow::gotoClicked()
{
    qDebug() << "Goto";

    bool isNumber;

    auto blockNumber = ui->gotoLineEdit->text().toULongLong(&isNumber);

    if(!isNumber)
    {
        qDebug() << "Incorrect value";
        ui->gotoLineEdit->clear();
        return;
    }

    if(blockNumber > g_blockchain.getBlocksCount() - 1)
    {
        qDebug() << "Nu such block";
        QMessageBox::critical(this, "No such block", "You entered incorrect block number\nLast block number is " + QString::number(g_blockchain.getBlocksCount() - 1));
        ui->gotoLineEdit->clear();
        return;
    }

    bool isThirdFieldNotEmpty;
    auto thirdFieldNumber = ui->numberLineEdit3->text().toULongLong(&isThirdFieldNotEmpty);

    if(!isThirdFieldNotEmpty)
    {
        qDebug() << "Third field is empty. Skip goto";
        ui->gotoLineEdit->clear();
        return;
    }

    //TODO : Implement it without calling signals. But now i want to sleep
    while(blockNumber > thirdFieldNumber++)
    {
        qDebug() << "Requested block is on the right";
        this->nextBlockClicked();

        thirdFieldNumber = ui->numberLineEdit3->text().toULongLong(&isThirdFieldNotEmpty);
    }

    auto firstFieldNumber = ui->numberLineEdit1->text().toULongLong(&isThirdFieldNotEmpty);

    while(blockNumber < firstFieldNumber--)
    {
        qDebug() << "Requested block is on the left";
        this->prevBlockClicked();

        //thirdFieldNumber = ui->numberLineEdit1->text().toULongLong();
    }
    ui->gotoLineEdit->clear();
}

void MainWindow::activate1()
{
    if(ui->numberLineEdit1->text().isEmpty())
    {
        return;
    }
    qDebug() << "Try to activate 1";
    CryptoPP::RSA::PrivateKey privateKey;

    auto privateKeyPath = QFileDialog::getOpenFileName(this, "Open private key", QDir::currentPath());

    BlockchainFile::LoadHexPrivateKey(privateKeyPath.toStdString(), privateKey);

    size_t blockNumber = ui->numberLineEdit1->text().toULongLong();

    CryptoPP::RSA::PublicKey publicKey;
    BlockchainFile::HexToRSAPublicKey(ui->addressLineEdit1->text().toStdString(), publicKey);
    if(::VerifyRSAKeyPair(publicKey, privateKey, g_blockchain.getBlockById(blockNumber)))
    {
        qDebug() << "Valid key pair. Block " << blockNumber << " successfully activated";
        g_blockchain.getBlockById(blockNumber).authorize();
        ui->authorizeBlockPushButton1->setDisabled(true);
        ui->authorizeBlockPushButton1->setText("Authorized");
    }
    else
    {
        qDebug() <<"Authorization for block " << blockNumber << " failed";
        QMessageBox::warning(this, "Incorrect key", "This key is not correct. Access denied");
        return;
    }
}

void MainWindow::activate2()
{
    if(ui->numberLineEdit2->text().isEmpty())
    {
        return;
    }
    qDebug() << "Try to activate 2";
    CryptoPP::RSA::PrivateKey privateKey;

    auto privateKeyPath = QFileDialog::getOpenFileName(this, "Open private key", QDir::currentPath());

    BlockchainFile::LoadHexPrivateKey(privateKeyPath.toStdString(), privateKey);

    size_t blockNumber = ui->numberLineEdit2->text().toULongLong();

    CryptoPP::RSA::PublicKey publicKey;
    BlockchainFile::HexToRSAPublicKey(ui->addressLineEdit2->text().toStdString(), publicKey);
    if(::VerifyRSAKeyPair(publicKey, privateKey, g_blockchain.getBlockById(blockNumber)))
    {
        qDebug() << "Valid key pair. Block " << blockNumber << " successfully activated";
        g_blockchain.getBlockById(blockNumber).authorize();
        ui->authorizeBlockPushButton2->setDisabled(true);
        ui->authorizeBlockPushButton2->setText("Authorized");
    }
    else
    {
        qDebug() <<"Authorization for block " << blockNumber << " failed";
        QMessageBox::warning(this, "Incorrect key", "This key is not correct. Access denied");
        return;
    }
}

void MainWindow::activate3()
{
    if(ui->numberLineEdit3->text().isEmpty())
    {
        return;
    }
    qDebug() << "Try to activate 3";
    CryptoPP::RSA::PrivateKey privateKey;

    auto privateKeyPath = QFileDialog::getOpenFileName(this, "Open private key", QDir::currentPath());

    BlockchainFile::LoadHexPrivateKey(privateKeyPath.toStdString(), privateKey);
    CryptoPP::AutoSeededRandomPool rng;
    privateKey.Validate(rng, 3);
    size_t blockNumber = ui->numberLineEdit3->text().toULongLong();

    CryptoPP::RSA::PublicKey publicKey;
    BlockchainFile::HexToRSAPublicKey(ui->addressLineEdit3->text().toStdString(), publicKey);
    if(::VerifyRSAKeyPair(publicKey, privateKey, g_blockchain.getBlockById(blockNumber)))
    {
        qDebug() << "Valid key pair. Block " << blockNumber << " successfully activated";
        g_blockchain.getBlockById(blockNumber).authorize();
        ui->authorizeBlockPushButton3->setDisabled(true);
        ui->authorizeBlockPushButton3->setText("Authorized");
    }
    else
    {
        qDebug() <<"Authorization for block " << blockNumber << " failed";
        QMessageBox::warning(this, "Incorrect key", "This key is not correct. Access denied");
        return;
    }
}

void MainWindow::showTransactions1()
{
    qDebug () << "Show transactions 1";
    auto blockNumber = ui->numberLineEdit1->text().toULongLong();
    ManageBlockTransactions(g_blockchain.getBlockById(blockNumber));
}

void MainWindow::showTransactions2()
{
    qDebug () << "Show transactions 2";
    auto blockNumber = ui->numberLineEdit2->text().toULongLong();
    ManageBlockTransactions(g_blockchain.getBlockById(blockNumber));
}

void MainWindow::showTransactions3()
{
    qDebug () << "Show transactions 3";
    auto blockNumber = ui->numberLineEdit3->text().toULongLong();
    ManageBlockTransactions(g_blockchain.getBlockById(blockNumber));
}
