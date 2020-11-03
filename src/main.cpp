#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlContext>
//#include "fluxhub.h"
#include <QtGui>
#include <QApplication>
#include <QImage>
#include <QLabel>

int main(int argc, char *argv[]) {
//    QGuiApplication app(argc, argv);

    QApplication app(argc, argv);

    QImage image;
    image.load("../resources/test.jpeg");

    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(image));

    myLabel.show();
//
//    QQmlApplicationEngine engine;
//    QScopedPointer<FluxHub> hub(new FluxHub(&engine));
//    engine.rootContext()->setContextProperty("$hub", hub.get());
//
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//                if (!obj && url == objUrl)
//                    QCoreApplication::exit(-1);
//            }, Qt::QueuedConnection);
//    engine.load(url);

    return app.exec();
}
