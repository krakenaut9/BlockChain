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
    std::string hexAddress;
    BlockchainFile::RSAPublicKeyToHex(m_pBlock->getAddress(), hexAddress);
    ui->blockAddressLineEdit->setText(QString::fromStdString(hexAddress));
    ui->blockAddressLineEdit->setReadOnly(true);
    setWindowTitle("Block " + QString::number(m_pBlock->getBlockNumber()) + " transactions");
}

TransactionsWindow::~TransactionsWindow()
{
    delete ui;
}
