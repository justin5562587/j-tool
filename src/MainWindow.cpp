//
// Created by justin on 2020/11/03.
//
#include "../include/MainWindow.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

#include <iostream>

MainWindow::MainWindow() : QMainWindow() {
    createContactMenus();
    createImageBrowserMenus();
    createExtraMenus();
    setWindowTitle("J-Tool");

    QLabel* label = new QLabel();
    QPixmap img("../resources/test.jpeg");
    label->setPixmap(img);
    setCentralWidget(label);
}

void MainWindow::createImageBrowserMenus() {
    QMenu *openMenu = menuBar()->addMenu("Image Browser");

    openImageAct = new QAction("Select image", this);
    openImageAct->setStatusTip("Select single image then show");
    connect(openImageAct, &QAction::triggered, this, &MainWindow::openAndShowImage);

    openMenu->addAction(openImageAct);
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

void MainWindow::createExtraMenus() {
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
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

void MainWindow::openAndShowImage() {
    // todo
}

// extra

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"), tr("J-Tool is a highly integrated desktop application designed for programmer"));
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
