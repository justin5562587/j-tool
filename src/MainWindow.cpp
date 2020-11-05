//
// Created by justin on 2020/11/03.
//
#include "../include/MainWindow.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

MainWindow::MainWindow() : QMainWindow() {
    createContactMenus();
    createImageBrowserMenus();
    setWindowTitle("J-Tool");
}

void MainWindow::createImageBrowserMenus() {
    QMenu *openMenu = menuBar()->addMenu("Image Browser");

    selectImageAct = new QAction("Select image", this);
    selectImageAct->setStatusTip("Select single image then show");
    connect(selectImageAct, &QAction::triggered, this, &MainWindow::selectImageAndShow);

    selectImageFolderAct = new QAction("Select folder", this);
    selectImageFolderAct->setStatusTip("Select a folder then show images in it");
    connect(selectImageFolderAct, &QAction::triggered, this, &MainWindow::selectFolderAndShow);

    openMenu->addAction(selectImageAct);
    openMenu->addAction(selectImageFolderAct);
}

void MainWindow::createContactMenus() {
    QMenu *openMenu = menuBar()->addMenu("Contact Menu");

    showContactMenuAct = new QAction("Show Contact Menu", this);
    showContactMenuAct->setStatusTip("Display all Contact Menu");
    connect(showContactMenuAct, &QAction::triggered, this, &MainWindow::showContactMenu);

    saveContactMenuAct = new QAction("Save Contact Menu", this);
    saveContactMenuAct->setStatusTip("Save current status of Contact Menu");
    connect(saveContactMenuAct, &QAction::triggered, this, &MainWindow::saveContactMenu);

    loadContactMenuAct = new QAction("Load Contact Menu", this);
    loadContactMenuAct->setStatusTip("Load data of Contact Menu from file");
    connect(loadContactMenuAct, &QAction::triggered, this, &MainWindow::loadContactMenu);

    openMenu->addAction(showContactMenuAct);
    openMenu->addAction(saveContactMenuAct);
    openMenu->addAction(loadContactMenuAct);
}

void MainWindow::showContactMenu() {
    QMessageBox msgBox;
    msgBox.setText("Coming soon...");
    msgBox.exec();
}

void MainWindow::saveContactMenu() {
    QMessageBox msgBox;
    msgBox.setText("Coming soon...");
    msgBox.exec();
}

void MainWindow::loadContactMenu() {
    QString filename = QFileDialog::getOpenFileName(this);
    if (!filename.isEmpty()) {
        std::cout << filename.toStdString() << std::endl;
    };
}

void MainWindow::selectImageAndShow() {
    QString filename = 
}

void MainWindow::selectFolderAndShow() {

}


//
//MainWindow::MainWindow() : QMainWindow(), addressWidget(new AddressWidget) {
//    setCentralWidget(addressWidget);
//    createMenus();
//    setWindowTitle(tr("Address Book"));
//}
//
//void MainWindow::createMenus() {
//    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
//    QAction *openAct = new QAction(tr("&Open..."), this);
//    fileMenu->addAction(openAct);
//    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
//
//    QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));
//
//    editAct = new QAction(tr("&Edit Entry..."), this);
//    editAct->setEnabled(false);
//    toolMenu->addAction(editAct);
//    connect(editAct, &QAction::triggered, addressWidget, &AddressWidget::editEntry);
//
//    toolMenu->addSeparator();
//
//    removeAct = new QAction(tr("&Remove Entry"), this);
//    removeAct->setEnabled(false);
//    toolMenu->addAction(removeAct);
//    connect(removeAct, &QAction::triggered, addressWidget, &AddressWidget::removeEntry);
//
//    connect(addressWidget, &AddressWidget::selectionChanged,this, &MainWindow::updateActions);
//}

//void MainWindow::openFile() {
//    QString filename = QFileDialog::getOpenFileName(this);
//    if (!filename.isEmpty()) {
//        addressWidget->readFromFile(filename);
//    }
//}
//
//void MainWindow::saveFile() {
//    QString filename = QFileDialog::getOpenFileName(this);
//    if (!filename.isEmpty()) {
//        addressWidget->writeToFile(filename);
//    }
//}
//
//void MainWindow::updateActions(const QItemSelection &selection) {
//    QModelIndexList indexes = selection.indexes();
//    if (!indexes.isEmpty()) {
//        removeAct->setEnabled(true);
//        editAct->setEnabled(true);
//    } else {
//        removeAct->setEnabled(false);
//        editAct->setEnabled(false);
//    }
//}