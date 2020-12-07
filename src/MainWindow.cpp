//
// Created by justin on 2020/11/03.
//
#include "./MainWindow.h"

#include <QMenu>
#include <QMessageBox>
#include <QHBoxLayout>

MainWindow::MainWindow() : QMainWindow(), imageBrowser(), todoList(), multimediaPlayer() {
    imageBrowserWidget = imageBrowser.getSelfWidget();
    todoListWidget = todoList.getSelfWidget();
    multimediaPlayerWidget = multimediaPlayer.getSelfWidget();

    createMenus();
    setWindowTitle("J-Tool");

//    centralWidget = new QWidget();
//    mainLayout = new QVBoxLayout;

    setCentralWidget(multimediaPlayerWidget);
}

void MainWindow::createMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Model");
    mainMenu->addAction("Image Browser", this, &MainWindow::setWithImageBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction("Todo List", this, &MainWindow::setWithTodoList);
    mainMenu->addSeparator();
    mainMenu->addAction("Multimedia Player", this, &MainWindow::setWithMultimediaPlayer);

    // help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

// slot
void MainWindow::setWithImageBrowser() {
    setCentralWidget(imageBrowserWidget);
}

void MainWindow::setWithTodoList() {
    setCentralWidget(todoListWidget);
}

void MainWindow::setWithMultimediaPlayer() {
    setCentralWidget(multimediaPlayerWidget);
}

//void MainWindow::removeChildWidgets() {
//    QList<QWidget *> widgets = findChildren<QWidget *>();
//    foreach(QWidget * widget, widgets){
//        delete widget;
//    }
//}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application designed for programmer"));
}
