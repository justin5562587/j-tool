//
// Created by justin on 2020/11/03.
//
#include "../include/MainWindow.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow() : QMainWindow() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QMenu *newMenu = menuBar()->addMenu(tr("&New"));

    QMenu *closeMenu = menuBar()->addMenu(tr("&Close"));


//    QMenu *newMenu = menuBar()->addMenu(tr("&New"));
//    newAct = new QAction(tr("&New"), this);
//    newAct->setStatusTip("Create a New File");
//    newAct->setShortcut(QKeySequence::New);
//    newMenu->addAction(newAct);
//    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
//
//    QMenu *openMenu = menuBar()->addMenu(tr("&Open"));
//    openAct = new QAction(tr("&Open"), this);
//    openAct->setStatusTip("Open a File");
//    openAct->setShortcut(QKeySequence::Open);
//    openMenu->addAction(openAct);
//    connect(openAct, &QAction::triggered, this, &MainWindow::open);

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
//
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