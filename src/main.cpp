#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlContext>
//#include "fluxhub.h"
#include <QImage>
#include <QLabel>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QImage image;
    image.load("");

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
