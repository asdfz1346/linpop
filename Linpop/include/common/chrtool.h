
#ifndef LINPOP_CHRTOOL_H
#define LINPOP_CHRTOOL_H

#include <singleton.h>

#include <iostream>

#include <QString>
#include <QMap>

class ChrTool : public Singleton<ChrTool> {

public:
    ~ChrTool();
    void encryptStr(const std::string &rsSrc, QString &rsDest);
    void decryptStr(const std::string &crsCipher, QString &rtPlain);
    void decryptAll(const QStringList &rtCipherList);


protected:
    friend class Singleton<ChrTool>;
    ChrTool();

private:
    QMap<char, int> m_tEncryptTable;
    QMap<int, char> m_tDecryptTable;
};

#endif // LINPOP_CHRTOOL_H
