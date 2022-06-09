#pragma once
#include <QDialog>
#include <blockchain.h>
#include <blockchainfile.h>

namespace Ui {
class TransactionsWindow;
}

class TransactionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionsWindow(Blockchain::Block* pBlock, QWidget *parent = nullptr);
    ~TransactionsWindow();

private:
    Ui::TransactionsWindow *ui;
    Blockchain::Block* m_pBlock;
};
