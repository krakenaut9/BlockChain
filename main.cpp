#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <blockchain.h>

int main(int argc, char *argv[])
{
    Blockchain::Transaction transaction("Hello world");
    Blockchain::Block block(0, "111", "222");
    block.addTransaction(transaction);
    block.addTransaction(Blockchain::Transaction("Second transaction"));
    block.complete();
    QApplication a(argc, argv);

    QTranslator translator;    
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Blockchain_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
