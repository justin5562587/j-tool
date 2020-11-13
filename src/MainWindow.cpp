//
// Created by justin on 2020/11/03.
//
#include "../include/MainWindow.h"

#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

MainWindow::MainWindow() : QMainWindow(), imageBrowser() {
    createMenus();
    setWindowTitle("J-Tool");

//    QWidget* centralArea = new QWidget();
//    centralArea->setLayout();

    setCentralWidget(imageBrowser.getSelfWidget());
}

void MainWindow::createMenus() {
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

// slot
void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"), tr("J-Tool is a highly integrated desktop application designed for programmer"));
}
