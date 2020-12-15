//
// Created by justin on 2020/11/03.
//
#include "./MainWindow.h"

#include <QMenu>
#include <QMessageBox>
#include <QDirIterator>
#include <QStackedWidget>

MainWindow::MainWindow() : QMainWindow() {
    stackedWidget = new QStackedWidget(this);
    m_multimediaPlayer = new MultimediaPlayer(this);
//    m_imageBrowser = new ImageBrowser(this);
//    m_todoList = new TodoList(this);
    stackedWidget->addWidget(m_multimediaPlayer);
//    stackedWidget->addWidget(m_imageBrowser);
//    stackedWidget->addWidget(m_todoList);

    createMenus();
    setWindowTitle("J-Tool");
    setCentralWidget(stackedWidget);
}

void MainWindow::createMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Models");
//    mainMenu->addAction("Image Browser", this, &MainWindow::setCentralWithImageBrowser);
//    mainMenu->addSeparator();
//    mainMenu->addAction("Todo List", this, &MainWindow::setCentralWithTodoList);
//    mainMenu->addSeparator();
    mainMenu->addAction("Multimedia Player", this, &MainWindow::setCentralWithMultimediaPlayer);

    // help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);

    // create menus of cores
    createMenusForMultimediaPlayer();
}

void MainWindow::createMenusForMultimediaPlayer() {
    QMenu *menu = menuBar()->addMenu("Multimedia Player");
    menu->addAction("Open", m_multimediaPlayer, &MultimediaPlayer::open);
}

// slot
void MainWindow::setCentralWithMultimediaPlayer() {
    stackedWidget->setCurrentIndex(0);
}

//void MainWindow::setCentralWithImageBrowser() {
//    stackedWidget->setCurrentIndex(1);
//}
//
//void MainWindow::setCentralWithTodoList() {
//    stackedWidget->setCurrentIndex(2);
//}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application designed for programmer"));
}
