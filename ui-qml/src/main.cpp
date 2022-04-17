#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include <QtMqtt/QtMqtt>

#include "qmlaes.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "leipoa_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/leipoa/views/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Test qmlaes
    QString key = "r]g)pm'y]1t?v`dfe[j/5g9b&&3(%hca";
    QString plainText = "This is a secret message ;)";
    QByteArray encryptedText = QmlAES::encrypt(plainText, key).toByteArray();
    // QString decyptedText =
    QmlAES::decrypt(encryptedText, key).toString();

    return app.exec();
}
