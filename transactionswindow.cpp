#include "transactionswindow.h"
#include "ui_transactionswindow.h"

TransactionsWindow::TransactionsWindow(Blockchain::Block* pBlock, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionsWindow),
    m_pBlock(pBlock)
{
    ui->setupUi(this);
    ui->blockNumberLineEdit->setText(QString::number(m_pBlock->getBlockNumber()));
    ui->blockNumberLineEdit->setReadOnly(true);
    ui->blockNumberLineEdit->setCursorPosition(0);
    std::string hexAddress;
    BlockchainFile::RSAPublicKeyToHex(m_pBlock->getAddress(), hexAddress);
    ui->blockAddressLineEdit->setText(QString::fromStdString(hexAddress));
    ui->blockAddressLineEdit->setReadOnly(true);
    ui->blockAddressLineEdit->setCursorPosition(0);

    for(const auto& transaction : pBlock->getTransactions())
    {
        auto transactionItem = new QTreeWidgetItem();
        transactionItem->setText(0, QString::number(transaction.getNumber()));

        auto informationItem = new QTreeWidgetItem(transactionItem);
        informationItem->setText(1, Blockchain::Transaction::Properties::information.c_str());
        informationItem->setText(2, QString::fromStdString(transaction.getInformation()));
        transactionItem->addChild(informationItem);

        auto signatureItem = new QTreeWidgetItem(transactionItem);
        signatureItem->setText(1, Blockchain::Transaction::Properties::digitalSignature.c_str());
        std::ostringstream ret;
        auto signature = transaction.getSignature();
        for (std::string::size_type i = 0; i < signature.length(); ++i)
            ret << std::hex << std::setfill('0') << std::setw(2) << (int)signature[i];
        signatureItem->setText(2, ret.str().c_str());
        transactionItem->addChild(signatureItem);

        auto timeItem = new QTreeWidgetItem(transactionItem);
        timeItem->setText(1, Blockchain::Transaction::Properties::time.c_str());
        timeItem->setText(2, QString::fromStdString(transaction.getTime()));
        transactionItem->addChild(timeItem);

        ui->treeWidget->addTopLevelItem(transactionItem);
    }

    setWindowTitle("Block " + QString::number(m_pBlock->getBlockNumber()) + " transactions");
}

TransactionsWindow::~TransactionsWindow()
{
    delete ui;
}
