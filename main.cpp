#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <blockchain.h>

int main(int argc, char *argv[])
{
//    auto keys = Cryptography::GenerateRSAKeyPair();
//    auto privatekey = keys.second;
//    Blockchain::Block block(0,"", keys.first);
//    block.setPrivateKey(keys.second);
//    std::cout << "1d = " << block.getPrivateKey().GetPrivateExponent() << "\n1e = " << block.getPrivateKey().GetPublicExponent()
//              << "\n1mod = " << block.getPrivateKey().GetModulus() << std::endl;
//    Blockchain::Transaction tr("Hello", block.getPrivateKey());
    //block.addTransaction(Blockchain::Transaction("Hello", ))
//    std::cout << "1d = " << keys.second.GetPrivateExponent() << "\n1e = " << keys.second.GetPublicExponent() << "\n1mod = " << keys.second.GetModulus() << std::endl;
//    std::cout << "2d = " << privatekey.GetPrivateExponent() << "\n2e = " << privatekey.GetPublicExponent() << "\n2mod = " << privatekey.GetModulus() << std::endl;
    //return 1;
    /*

    std::string message = "Dima maslo";
    CryptoPP::Integer intMessage((const CryptoPP::byte *)message.data(), message.size());
    auto keys = Cryptography::GenerateRSAKeyPair();
    auto intEncrypted = keys.first.ApplyFunction(intMessage);
    auto sign = Cryptography::SignData(message, keys.second);
    CryptoPP::AutoSeededRandomPool rng;
    auto keys2 = Cryptography::GenerateRSAKeyPair();

    Cryptography::CheckSignature(message, sign, keys2.first);
    auto intRecoveredMessage = keys2.second.CalculateInverse(rng, intEncrypted);
    std::cout << "1 : d = " << keys.second.GetPrivateExponent() << "\nmod = " << keys.second.GetModulus() << std::endl;
    std::cout << "2 : d = " << keys2.second.GetPrivateExponent() << "\nmod = " << keys2.second.GetModulus() << std::endl;
    std::string recoveredMessage;
    recoveredMessage.resize(intRecoveredMessage.MinEncodedSize());
    intRecoveredMessage.Encode((CryptoPP::byte *) &recoveredMessage[0], recoveredMessage.size());
    qDebug() << QString::fromStdString(recoveredMessage);
    return recoveredMessage == message;*/
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
