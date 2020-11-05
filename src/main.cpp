#include <QApplication>
//#include "../include/MainWindow.h"

#include "../include/ImageViewer.h"

int main(int argc, char *argv[]) {

//#ifdef Q_OS_ANDROID
//    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif
    QApplication app(argc, argv);
//    MainWindow mw;
//    mw.show();
    ImageViewer imageViewer;
    imageViewer.show();
    return app.exec();
}
