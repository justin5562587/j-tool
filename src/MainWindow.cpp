//
// Created by justin on 2020/11/03.
//
#include "./MainWindow.h"

#include <QMenu>
#include <QMessageBox>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

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
    m_signalMapper = new QSignalMapper(this);

    QAction* mainMenuAction1 = mainMenu->addAction("Main Window");
    mainMenuAction1->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    connect(mainMenuAction1, SIGNAL(triggered()), m_signalMapper, SLOT(map()));

    QAction* mainMenuAction2 = mainMenu->addAction("Image Browser");
    mainMenuAction2->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    connect(mainMenuAction2, SIGNAL(triggered()), m_signalMapper, SLOT(map()));

    QAction* mainMenuAction3 = mainMenu->addAction("Todo List");
    mainMenuAction3->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    connect(mainMenuAction3, SIGNAL(triggered()), m_signalMapper, SLOT(map()));

    QAction* mainMenuAction4 = mainMenu->addAction("Multimedia Player");
    mainMenuAction4->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));
    connect(mainMenuAction4, SIGNAL(triggered()), m_signalMapper, SLOT(map()));

    m_signalMapper->setMapping(mainMenuAction1, 0);
    m_signalMapper->setMapping(mainMenuAction2, 1);
    m_signalMapper->setMapping(mainMenuAction3, 1);
    m_signalMapper->setMapping(mainMenuAction4, 1);
    connect (m_signalMapper, SIGNAL(mapped(int)), this, SLOT(setCentralWidgetWith(int))) ;

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
void MainWindow::setCentralWidgetWith(int stackedWidgetIndex) {
    m_stackedWidget->setCurrentIndex(stackedWidgetIndex);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About J-Tool"),
                       tr("J-Tool is a highly integrated desktop application with multiple functionalities designed for programmer, officer and etc."));
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
        connect(toBtn, &QAbstractButton::clicked, this, &MainWindow::setCentralWidgetWith);

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