//
// Created by justin on 2020/11/03.
//
#include "./MainWindow.h"

#include <QMenu>
#include <QMessageBox>
#include <QDirIterator>
#include <QStackedWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow() : QMainWindow() {
    QFile styleFile(":/qss/global.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    setStyleSheet(styleSheet);

    renderInfoSections();

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_infoWrapper);

    m_multimediaPlayer = new MultimediaPlayer(this);
    m_stackedWidget->addWidget(m_multimediaPlayer);

//    m_imageBrowser = new ImageBrowser(this);
//    m_stackedWidget->addWidget(m_imageBrowser);

//    m_todoList = new TodoList(this);
//    m_stackedWidget->addWidget(m_todoList);

    createMenus();
    setWindowTitle("J-Tool");
    setCentralWidget(m_stackedWidget);
}

void MainWindow::createMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Models");
    mainMenu->addAction("Main Window", this, &MainWindow::setCentralWithInfoWrapper);
    mainMenu->addSeparator();
    mainMenu->addAction("Image Browser", this, &MainWindow::setCentralWithImageBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction("Todo List", this, &MainWindow::setCentralWithTodoList);
    mainMenu->addSeparator();
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
void MainWindow::setCentralWithInfoWrapper() {
    m_stackedWidget->setCurrentIndex(0);
}

void MainWindow::setCentralWithMultimediaPlayer() {
    m_stackedWidget->setCurrentIndex(1);
}

void MainWindow::setCentralWithImageBrowser() {
    m_stackedWidget->setCurrentIndex(1);
}

void MainWindow::setCentralWithTodoList() {
    m_stackedWidget->setCurrentIndex(1);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application designed for programmer"));
}

void MainWindow::renderInfoSections() {
    QHBoxLayout *layout = new QHBoxLayout;

    QFile file(":/json/info.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    QJsonArray items = jsonObject.value("coreInfos").toArray();

    auto begin = items.begin();
    auto end = items.end();
    while(begin != end) {
        auto item = (*begin).toObject();

        QWidget* infoSection= new QWidget;
        infoSection->setFixedHeight(250);
        infoSection->setFixedWidth(450);
        infoSection->setObjectName("infoSection");
        QVBoxLayout *sectionLayout = new QVBoxLayout;
        infoSection->setLayout(sectionLayout);

        QLabel *title = new QLabel(item["title"].toString());
        title->setObjectName("infoTitle");

        QLabel *description = new QLabel(item["description"].toString());
        description->setObjectName("infoDescription");
        description->setWordWrap(true);

        QLabel *latestUpdate = new QLabel("Last Update: " + item["lastUpdate"].toString());
        latestUpdate->setObjectName("infoTime");

        QPushButton *toBtn = new QPushButton("Goto");
        toBtn->setObjectName("infoToBtn");
        connect(toBtn, &QAbstractButton::clicked, this, &MainWindow::setCentralWithMultimediaPlayer);

        sectionLayout->addWidget(title);
        sectionLayout->addWidget(description);
        sectionLayout->addWidget(latestUpdate);
        sectionLayout->addWidget(toBtn);

        layout->addWidget(infoSection);
        layout->addStretch();
        ++begin;
    }

    m_infoWrapper = new QWidget(this);
    m_infoWrapper->setObjectName("infoWrapper");
    m_infoWrapper->setLayout(layout);
}