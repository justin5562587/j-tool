#include <QApplication>
#include "../include/MainWindow.h"

int main(int argc, char *argv[]) {

#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}
