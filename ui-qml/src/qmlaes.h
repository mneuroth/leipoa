#ifndef QMLAES_H
#define QMLAES_H

#include <QObject>
#include <QVariant>

class QmlAES : public QObject
{
    Q_OBJECT
public:
    QmlAES(QObject *parent = nullptr);
    static Q_INVOKABLE QVariant encrypt(QString plainText, QString key);
    static Q_INVOKABLE QVariant decrypt(QByteArray encodedText, QString key);
};

#endif // QMLAES_H
