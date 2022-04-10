#include "qmlaes.h"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

// Derived from: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

void handleErrors(void)
{
}

int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

QmlAES::QmlAES(QObject *parent) : QObject(parent)
{
}

void qDebugString(const QString s, const QString a) {
    qDebug("[DEBUG] %s: %s",
          s.toStdString().c_str(),
          a.toStdString().c_str());
}

void qDebugByteArray(const QString s, const QByteArray a) {
    qDebug("[DEBUG] %s: %s",
          s.toStdString().c_str(),
          a.toBase64().toStdString().c_str());
}

#define IV_LENGTH 16

QVariant QmlAES::encrypt(QString plainText, QString key)
{
    // plain text
    QByteArray plainTextB = plainText.toUtf8();
    int plainTextULen = plainTextB.size();
    unsigned char *plainTextU = new unsigned char[plainTextULen+1];
    memcpy(plainTextU, plainTextB.data(), plainTextULen);
    plainTextU[plainTextULen] = 0;

    // key (32 bytes)
    QByteArray keyB = key.toUtf8().append(32,'#').mid(0,32);
    int keyULen = keyB.size();
    qDebug("AAA %d", keyULen);
    unsigned char *keyU = new unsigned char[keyULen+1];
    memcpy(keyU, keyB.data(), keyULen+1);
    keyU[keyULen] = 0;

    // IV
    int ivULen = IV_LENGTH;
    unsigned char ivU[ivULen+1];
    RAND_bytes(ivU, ivULen);
    ivU[ivULen] = 0;
    QByteArray ivB = QByteArray::fromRawData((const char*)ivU, sizeof(ivU));

    // encrypt plain text to cipher text
    unsigned char cipherTextU[plainTextULen + EVP_MAX_BLOCK_LENGTH + 1];
    int cipherTextULen = aes_encrypt (
                plainTextU, strlen((char *)plainTextU),
                keyU, ivU, cipherTextU);
    cipherTextU[cipherTextULen] = 0;
    QByteArray cipherTextB = QByteArray::fromRawData((const char *)cipherTextU, cipherTextULen);

    qDebug("[DEBUG] (1) Ciphertext length: %d=%d", cipherTextULen, (int)strlen((char*)cipherTextU));
    qDebugString(   "(1) Plain text       ", plainText);
    qDebugString(   "(1) Key              ", key);
    qDebugString(   "(1) Key              ", keyB.data());
    qDebugByteArray("(1) Bytes plain text ", plainTextB);
    qDebugByteArray("(1) Bytes key        ", keyB);
    qDebugByteArray("(1) Bytes IV         ", ivB);
    qDebugByteArray("(1) Bytes cipher text", cipherTextB);

    delete[](plainTextU);
    delete[](keyU);

    QByteArray encodedTextB = ivB + cipherTextB;
    QByteArray encodedTextB64 = encodedTextB.toBase64();

    return QVariant::fromValue(encodedTextB64);
}

QVariant QmlAES::decrypt(QByteArray encodedTextB64, QString key)
{
    QByteArray encodedText = QByteArray::fromBase64(encodedTextB64);

    QByteArray keyB2 = key.toUtf8().append(32,'#').mid(0,32);
    QByteArray ivB2 = encodedText.mid(0, IV_LENGTH+1);
    QByteArray cipherTextB2 = encodedText.mid(IV_LENGTH+1);

    qDebugByteArray("(3) Bytes key        ", keyB2);
    qDebugString(   "(3) Key              ", key);
    qDebugString(   "(3) Key              ", keyB2.data());
    qDebugByteArray("(3) Bytes IV         ", ivB2);
    qDebugByteArray("(3) Bytes cipher text", cipherTextB2);

    int keyU2Len = keyB2.size();
    unsigned char *keyU2 = new unsigned char[keyU2Len+1];
    memcpy(keyU2, keyB2.data(), keyU2Len+1);
    keyU2[keyU2Len] = 0;

    int ivU2Len = ivB2.size();
    unsigned char *ivU2 = new unsigned char[ivU2Len+1];
    memcpy(ivU2, ivB2.data(), ivU2Len+1);
    ivU2[ivU2Len] = 0;

    int cipherTextU2Len = cipherTextB2.size();
    unsigned char *cipherTextU2 = new unsigned char[cipherTextU2Len+1];
    memcpy(cipherTextU2, cipherTextB2.data(), cipherTextU2Len+1);
    cipherTextU2[cipherTextU2Len] = 0;

    unsigned char decryptedTextU[cipherTextU2Len+1];
    int decryptedTextULen = aes_decrypt(
                cipherTextU2, cipherTextU2Len,
                keyU2, ivU2,
                decryptedTextU);
    decryptedTextU[decryptedTextULen] = '\0';

    QByteArray decodedTextB = QByteArray::fromRawData((char*)decryptedTextU, decryptedTextULen);
    QString decodedText = QString::fromUtf8(decodedTextB);
    qDebugString("(3) Decoded text     ", decodedText);

    delete[](keyU2);
    delete[](ivU2);
    delete[](cipherTextU2);

    return QVariant::fromValue(decodedText);
}
