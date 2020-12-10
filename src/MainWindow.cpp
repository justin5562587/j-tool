//
// Created by justin on 2020/11/03.
//
#include "./MainWindow.h"

#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow() : QMainWindow() {
    m_imageBrowser = new ImageBrowser(this);
    m_todoList = new TodoList(this);
    m_multimediaPlayer = new MultimediaPlayer(this);

    createMenus();
    setWindowTitle("J-Tool");

    // default call setCentralWidget
    setCentralWithMultimediaPlayer();
}

void MainWindow::createMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Model");
    mainMenu->addAction("Image Browser", this, &MainWindow::setCentralWithImageBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction("Todo List", this, &MainWindow::setCentralWithTodoList);
    mainMenu->addSeparator();
    mainMenu->addAction("Multimedia Player", this, &MainWindow::setCentralWithMultimediaPlayer);

    // help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

// slot
void MainWindow::setCentralWithImageBrowser() {
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* centralLayout = new QVBoxLayout();
    centralLayout->addWidget(m_imageBrowser);
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::setCentralWithTodoList() {
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* centralLayout = new QVBoxLayout();
    centralLayout->addWidget(m_todoList);
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::setCentralWithMultimediaPlayer() {
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* centralLayout = new QVBoxLayout();
    centralLayout->addWidget(m_multimediaPlayer);
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application designed for programmer"));
}
