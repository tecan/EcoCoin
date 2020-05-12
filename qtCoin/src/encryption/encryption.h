#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "rsa/Rsa.h"
#include "Qt-AES/qaesencryption.h"
#include <QCryptographicHash>
#include <QString>
#include <QDebug>
#include <QFile>
#include "../../mainwindow.h"
#include "simpleCrypt/simplecrypt.h"
#include <QTextCodec>

//https://doc.qt.io/qt-5/qcryptographichash.html

QString MainWindow::simplecrypt(QString string,QString key,QCryptographicHash::Algorithm hash)
{

    QByteArray test = key.toUtf8();//0x0c2ad4a4acb9f023;
//convert to hex

    //Q_UINT64_C(test.toHex()
   uint test2= test.toHex().toInt();

//optional setkey method
 // SimpleCrypt crypto();
 // crypto.setKey(key);
  SimpleCrypt crypto(test2); //some random number
  QString result = crypto.encryptToString(string,hash);
  return result;
 }

QString MainWindow::simpledecrypt(QString string,QString key,QCryptographicHash::Algorithm hash)
{
    QByteArray test = key.toUtf8();//0x0c2ad4a4acb9f023;
//convert to hex

    //Q_UINT64_C(test.toHex()
   uint test2= test.toHex().toInt();

    //QByteArray::fromHex(key.toLatin1();
  //  QByteArray tester2= key.toUtf8();
  //   uint tester2= key.toUtf8();
  SimpleCrypt crypto(test2); //some random number
  QString decrypted = crypto.decryptToString(string,hash);
  return decrypted;
 }

QString MainWindow::rot13( const QString & input )
{//https://doc.qt.io/archives/3.3/rot-example.html
    QString r = input;
    int i = r.length();
    while( i-- ) {
        if ( r[i] >= QChar('A') && r[i] <= QChar('M') ||
             r[i] >= QChar('a') && r[i] <= QChar('m') )
            r[i] = (char)((int)QChar(r[i]).toLatin1() + 13);
        else if  ( r[i] >= QChar('N') && r[i] <= QChar('Z') ||
                   r[i] >= QChar('n') && r[i] <= QChar('z') )
            r[i] = (char)((int)QChar(r[i]).toLatin1() - 13);
    }
    return r;
}

QByteArray MainWindow::md5Checksum(QString stuff)
{
    //https://stackoverflow.com/questions/16383392/how-to-get-the-sha-1-md5-checksum-of-a-file-with-qt
    // Returns empty QByteArray() on failure.

    QCryptographicHash hash(QCryptographicHash::Md5); //QCryptographicHash::Md5
    hash.addData(stuff.toLatin1());
    return hash.result();

        //return QByteArray();
}

QByteArray MainWindow::fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm)
{
    //https://stackoverflow.com/questions/16383392/how-to-get-the-sha-1-md5-checksum-of-a-file-with-qt
    // Returns empty QByteArray() on failure.
        QFile f(fileName);
        if (f.open(QFile::ReadOnly)) {
            QCryptographicHash hash(hashAlgorithm); //QCryptographicHash::Md5
            if (hash.addData(&f)) {
                return hash.result();
            }
        }
        return QByteArray();
}

QString MainWindow::rsaenc(QString string2, Rsa *rsa)
{
    qDebug()<<"Encryption Start";

    string plainMsg=string2.toStdString();
    string encryptedMsg=rsaTester->rsa_MSG_encryp(plainMsg);
    return QString::fromStdString(encryptedMsg);
}

QString MainWindow::rsadec(QString string2, Rsa *rsa)
{
    qDebug()<<"Decryption Start";

    string encMsg = string2.toStdString();
    string decryptedMsg=rsa->rsa_MSG_decryp(encMsg);
    return QString(decryptedMsg.c_str());

 }


QByteArray MainWindow::aesenc(QString string2,QString skey,QString siv)
{
    QString key(skey);
    QString iv(siv);
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV =    QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encrypted = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC,string2.toLocal8Bit(), hashKey, hashIV, QAESEncryption::PKCS7);
    return encrypted;
}

QString MainWindow::aesdec(QByteArray string2,QString skey,QString siv )
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QString key(skey);
    QString iv(siv);
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV =    QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    //QByteArray encrypted = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC,inputStr.toLocal8Bit(), hashKey, hashIV); //... // Removal of Padding via Static function QString decodedString =
    QByteArray encrypted = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::CBC,string2, hashKey, hashIV);
    QByteArray encrypted1 = QAESEncryption::RemovePadding(encrypted, QAESEncryption::ISO);
    //QByteArray decodeText = encryption.decode(encrypted, hashKey, hashIV);
    QString decodedString = QString(encrypted1);

    return decodedString;
}

QString MainWindow::encdec2(QString string2,int encdec)
{
//    if (encdec == 0){
//    string2=rsadec(string2);
//    string2=aesdec(string2,"123456789","your-IV-vector");
//    qDebug() << "decrypted string is " << string2;
//        }
//    else {
//    string2=rsaenc(string2);
//    string2=aesenc(string2,"123456789","your-IV-vector");

//    qDebug() << "encrypted string is " << string2;
//    }
    return string2;
}

QString MainWindow::encdec(QString string2,int encdec)
{
    qDebug()<<"Encryption Start";

    string plainMsg=string2.toStdString();
    qDebug() << QString(plainMsg.c_str());

    Rsa rsaTester;
    string encryptedMsg=rsaTester.rsa_MSG_encryp(plainMsg);
    BigInt e, n;

    qDebug() << "rsaencrypted string" << QString(encryptedMsg.c_str());

    rsaTester.publish_keys(e, n);
    qDebug()<<QString(encryptedMsg.c_str());

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QString inputStr(QString::fromStdString(encryptedMsg));
    QString key("123456789");
    QString iv("your-IV-vector");
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV =    QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encrypted = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC,inputStr.toLocal8Bit(), hashKey, hashIV); //... // Removal of Padding via Static function QString decodedString =
    qDebug()<<"aes_encrypted string"<<encrypted;
    QString test2=QString(encrypted);
    QString test ;//= QString( "'" + QString::fromStdString(encrypted) + "'");
    test.append("'");
    test.append( test2 );
    test.append("'");


    qDebug() <<"testing string from qbytearray"<< test;

    QByteArray decodeText = encryption.decode(encrypted, hashKey, hashIV);
    QString decodedString = QString(encryption.removePadding(decodeText));
    string decoded = decodedString.toStdString();
    string decryptedMsg=rsaTester.rsa_MSG_decryp(decoded);
    qDebug()<<"aes->rsa decrypted message"<<QString(decryptedMsg.c_str());
    return (decryptedMsg.c_str());
}

QByteArray MainWindow::EncryptMsg(QString plainMsg,QString aeskey1,QString aeskey2)
{
    qDebug()<<"Encryption Started";
    QString enc1 = rsaenc(plainMsg);
   // QByteArray enc2 = aesenc(enc1, "123456789", "your-IV-vector");
    QByteArray enc2 = aesenc(enc1, aeskey1.toLatin1(), aeskey2.toLatin1());
    return enc2.toHex();
}

QString MainWindow::DecryptMsg(QByteArray encryptedMsg, Rsa *rsa,QString aeskey1,QString aeskey2)
{
    qDebug()<<"Decryption Started";
   // QString dec1 = aesdec(QByteArray::fromHex(encryptedMsg), "123456789", "your-IV-vector");
    QString dec1 = aesdec(QByteArray::fromHex(encryptedMsg), aeskey1.toLatin1(), aeskey2.toLatin1() );
    QString dec2 = rsadec(dec1, rsa);
    return dec2;
}




QString MainWindow::encryptxor(QString test,QString key)
{
//code taken from silve_smuggler xor utility
    // Read the file into a QByteArray
    QByteArray clearBlob = test.toLatin1();
    qDebug() << "File contains " << clearBlob.length() << " bytes.";

    // Convert the key into a QByteArray
    QByteArray keyBlob = key.toLatin1();

    qDebug() << "Key is " << keyBlob.length() << " bytes.";

    // XOR the file with the key
    QByteArray cryptBlob;
    // Give ourselves the memory before hand, so we don't call append() in the loop. It's slow.
    cryptBlob.resize(clearBlob.size());
    int blobPos;
    for (blobPos = 0; blobPos < clearBlob.length(); blobPos++)
    {
        // XOR each byte of the clearblob with the key, wrapping the key position around
        cryptBlob[blobPos] = clearBlob[blobPos] ^ keyBlob[blobPos % keyBlob.size()];
        //qDebug() << blobPos << ": " << clearBlob[blobPos] << " XOR " << keyBlob[blobPos % keyBlob.size()] << " (@ " << blobPos % keyBlob.size() << ")";
    }

    // Base64Encode the resulting encrypted QByteArray
    QString output = cryptBlob.toBase64(QByteArray::Base64Encoding | QByteArray::KeepTrailingEquals);

return output;
}

QString MainWindow::decryptxor(QString string,QString key)
{
    // Read the input into a QByteArray (integral Base64Decode)
    QByteArray cryptBlob;
    cryptBlob = QByteArray::fromBase64(string.toUtf8(), QByteArray::Base64Encoding | QByteArray::KeepTrailingEquals);
    qDebug() << "Input contains " << cryptBlob.length() << " bytes.";

    // Convert the key into a QByteArray
    QByteArray keyBlob = key.toLatin1();

    // XOR the key with the decoded input
    QByteArray clearBlob;
    // Give ourselves the memory before hand, so we don't call append() in the loop. It's slow.
    clearBlob.resize(cryptBlob.size());
    int blobPos;
    for (blobPos = 0; blobPos < cryptBlob.length(); blobPos++)
    {
        // XOR each byte of the clearblob with the key, wrapping the key position around
        clearBlob[blobPos] = cryptBlob[blobPos] ^ keyBlob[blobPos % keyBlob.size()];
        //qDebug() << blobPos << ": " << cryptBlob[blobPos] << " XOR " << keyBlob[blobPos % keyBlob.size()] << " (@ " << blobPos % keyBlob.size() << ")";
    }

    QTextCodec *codec = QTextCodec::codecForName("KOI8-R");

    return codec->toUnicode(clearBlob);
}

// old xor functions


//still trying to get xor working

//QString XORencryptDecrypt(QString toEncrypt) {
//    QChar key = 'K'; //Any char will work
//  //  QString output = toEncrypt;

//    QBbytearray output = qstringtobyte (toEncrypt);
//    for (int i = 0; i < toEncrypt.size(); i++)
//        output[i] = toEncrypt[i] ^ key;

//    return output;
//}



//string XOR(string value,string key)
//{
//    string retval(value);

//    short unsigned int klen=key.length();
//    short unsigned int vlen=value.length();
//    short unsigned int k=0;
//    short unsigned int v=0;

//    for(v = 0;v<vlen;v++)
//    {
//        retval[v]=value[v]^key[k];
//        k=(++k<klen?k:0);
//    }

//    return retval;
//}

//QString XORencryptDecrypt2(QString toEncrypt, QString key2)
//{
//    //QByteArray::toHex()
//QByteArray arr = toEncrypt.toLatin1();
//char key[3] = {'K', 'C', 'Q'};
//for (int i = 0; i < arr.size(); i++)
//    arr[i] = arr[i] ^ key[i % (sizeof(key) / sizeof(char))];

//}

//QString XORencryptDecrypt(QString toEncrypt, QString key2)
//{

//    const char* input = toEncrypt.toLatin1();
//    int inputLength = toEncrypt.size(); //m_textEdit->toPlainText().toLatin1().length();
//    const char* key = key2.toLatin1();//m_keyLineEdit->text().toLatin1().data();
//    int keyLength = key2.size();//m_keyLineEdit->text().toLatin1().length();

//    char output[inputLength];

//    for (int i = 0; i < inputLength + 1; ++i)
//    {
//        output[i] = input[i] ^ key[i % keyLength + 1];
//    }

////    if (strinfo.length () > 0) {
////    Qtextcodec *codec = Qtextcodec::codecforname ("Utf-8");
////    result = Codec->fromunicode (strinfo);
////    }


//    //m_textEdit->setText(QString::fromLatin1(output, inputLength));
//    return QString::fromLatin1(output, inputLength);
//}



#endif


