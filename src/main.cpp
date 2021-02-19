#include <iostream>
#include <QApplication>
#include <pthread.h>
#include "MainWindow.h"

#include "version.h"

int main(int argc, char *argv[]) {
//    std::cout << "Program Version: " << PROJECT_VERSION << std::endl;
    std::cout << "Git Hash: " << GIT_HASH << std::endl;

    qInfo() << "MultimediaPlayer thread ID: " << pthread_self();

    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}
