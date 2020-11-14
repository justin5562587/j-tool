//
// Created by justin on 2020/11/03.
//
#include "../include/MainWindow.h"

#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

MainWindow::MainWindow() : QMainWindow(), imageBrowser(), todoList() {
    imageBrowserWidget = imageBrowser.getSelfWidget();
    todoListWidget = todoList.getSelfWidget();

    createMenus();
    setWindowTitle("J-Tool");

    centralWidget = new QWidget();
    mainLayout = new QVBoxLayout;

    setWithImageBrowser();

    // set default widget
    setCentralWidget(centralWidget);
}

void MainWindow::createMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Model");
    mainMenu->addAction("Image Browser", this, &MainWindow::setWithImageBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction("Todo List", this, &MainWindow::setWithTodoList);

    // help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

// slot
void MainWindow::setWithImageBrowser() {
//    mainLayout->addWidget(imageBrowserWidget);
//    centralWidget->setLayout(mainLayout);
}

void MainWindow::setWithTodoList() {
//    mainLayout->addWidget(todoListWidget);
//    centralWidget->setLayout(mainLayout);
}

void MainWindow::removeChildWidgets() {
    QList<QWidget *> widgets = findChildren<QWidget *>();
    foreach(QWidget * widget, widgets){
        delete widget;
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application designed for programmer"));
}
