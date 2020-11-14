//
// Created by justin on 2020/11/03.
//
#include "../include/MainWindow.h"

#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

MainWindow::MainWindow() : QMainWindow(), imageBrowser(), todoList() {
    createMenus();
    setWindowTitle("J-Tool");

//    QWidget* centralArea = new QWidget();
//    centralArea->setLayout();

    // set default widget
    setCentralWidget(imageBrowser.getSelfWidget());
}

void MainWindow::createMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Model");
    mainMenu->addAction("Image Browser", this, &MainWindow::changeToImageBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction("Todo List", this, &MainWindow::changeToTodoList);

    // help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

// slot
void MainWindow::changeToImageBrowser() {
    setCentralWidget(imageBrowser.getSelfWidget());
}

void MainWindow::changeToTodoList() {
    setCentralWidget(todoList.getSelfWidget());
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application designed for programmer"));
}
