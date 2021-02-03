//
// Created by justin on 2020/11/03.
//
#include "MainWindow.h"

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

    initializeActions();
    initializeMenus();
    renderInfoSections();
    initializeWidget();
}

void MainWindow::initializeWidget() {
    m_stackedWidget = new QStackedWidget(this);

    m_stackedWidget->addWidget(m_infoWrapper);

    m_multimediaPlayer = new MultimediaPlayer(this);
    m_stackedWidget->addWidget(m_multimediaPlayer);

    m_pdfProcessor = new PDFMain(this);
    m_stackedWidget->addWidget(m_pdfProcessor);

    m_calendar = new Calendar(this);
    m_stackedWidget->addWidget(m_calendar);

    m_screenShot = new ScreenShot(this);
    m_stackedWidget->addWidget(m_screenShot);

    setWindowTitle("J-Tool");
    setCentralWidget(m_stackedWidget);
}

void MainWindow::initializeActions() {
    m_signalMapper = new QSignalMapper(this);
    m_toActVector = new QVector<QAction *>();

    QAction* toMainWindowAct = new QAction;
    toMainWindowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    connect(toMainWindowAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(toMainWindowAct);

    QAction* toMultimediaPlayerAct = new QAction();
    toMultimediaPlayerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    connect(toMultimediaPlayerAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(toMultimediaPlayerAct);

    QAction* toPdfProcessorAct = new QAction();
    toPdfProcessorAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    connect(toPdfProcessorAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(toPdfProcessorAct);

    QAction* toCalenderAct = new QAction();
    toCalenderAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    connect(toCalenderAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(toCalenderAct);

    QAction* toScreenShot = new QAction();
    toScreenShot->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));
    connect(toScreenShot, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(toScreenShot);

    m_signalMapper->setMapping(toMainWindowAct, 0);
    m_signalMapper->setMapping(toMultimediaPlayerAct, 1);
    m_signalMapper->setMapping(toPdfProcessorAct, 2);
    m_signalMapper->setMapping(toCalenderAct, 3);
    m_signalMapper->setMapping(toScreenShot, 4);

    connect(m_signalMapper, SIGNAL(mapped(int)), this, SLOT(setCentralWidgetWith(int)));
}

void MainWindow::initializeMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Models");

    mainMenu->addAction(m_toActVector->at(0));
    m_toActVector->at(0)->setText("Main Window");

    mainMenu->addSeparator();

    mainMenu->addAction(m_toActVector->at(1));
    m_toActVector->at(1)->setText("Multimedia Player");

    mainMenu->addAction(m_toActVector->at(2));
    m_toActVector->at(2)->setText("PDF Processor");

    mainMenu->addAction(m_toActVector->at(3));
    m_toActVector->at(3)->setText("Calender");

    mainMenu->addAction(m_toActVector->at(4));
    m_toActVector->at(4)->setText("ScreenShot");

    // help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);

    // create menus of cores
    createMenusForMultimediaPlayer();
}

void MainWindow::createMenusForMultimediaPlayer() {
    QMenu *menu = menuBar()->addMenu("Multimedia Player");
//    menu->addAction("Open", m_multimediaPlayer, &MultimediaPlayer::open);
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
    while (begin != end) {
        auto item = (*begin).toObject();

        QWidget *infoSection = new QWidget;
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
        connect(toBtn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
        m_signalMapper->setMapping(toBtn, item["stackedWidgetIndex"].toInt());

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