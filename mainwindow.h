#pragma once

#include <QMainWindow>
#include <cryptography.h>
#include <blockchain.h>
#include <blockchainfile.h>
#include <QFileDialog>

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
private:
    Ui::MainWindow *ui;
};
