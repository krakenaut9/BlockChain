#pragma once

#include <QMainWindow>
#include <cryptography.h>
#include <blockchain.h>
#include <blockchainfile.h>
#include <QFileDialog>
#include <QInputDialog>
#include <transactionswindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void newBlockClicked();
    void nextBlockClicked();
    void prevBlockClicked();
    void gotoClicked();
    void activate1();
    void activate2();
    void activate3();
    void showTransactions1();
    void showTransactions2();
    void showTransactions3();
    void addTransaction1();
    void addTransaction2();
    void addTransaction3();
private:
    Ui::MainWindow *ui;
};
