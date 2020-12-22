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

    setWindowTitle("J-Tool");
    setCentralWidget(m_stackedWidget);
}

void MainWindow::initializeActions() {
    m_signalMapper = new QSignalMapper(this);
    m_toActVector = new QVector<QAction *>();

    m_toMainWindowAct = new QAction;
    m_toMainWindowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    connect(m_toMainWindowAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(m_toMainWindowAct);

    m_toMultimediaPlayerAct = new QAction();
    m_toMultimediaPlayerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    connect(m_toMultimediaPlayerAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(m_toMultimediaPlayerAct);

    m_toPdfProcessorAct = new QAction();
    m_toPdfProcessorAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    connect(m_toPdfProcessorAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(m_toPdfProcessorAct);

    m_toCalenderAct = new QAction();
    m_toCalenderAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    connect(m_toCalenderAct, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_toActVector->push_back(m_toCalenderAct);

    m_signalMapper->setMapping(m_toMainWindowAct, 0);
    m_signalMapper->setMapping(m_toMultimediaPlayerAct, 1);
    m_signalMapper->setMapping(m_toPdfProcessorAct, 2);
    m_signalMapper->setMapping(m_toCalenderAct, 3);

    connect(m_signalMapper, SIGNAL(mapped(int)), this, SLOT(setCentralWidgetWith(int)));
}

void MainWindow::initializeMenus() {
    // main menu
    QMenu *mainMenu = menuBar()->addMenu("Models");

    mainMenu->addAction(m_toMainWindowAct);
    m_toMainWindowAct->setText("Main Window");

    mainMenu->addSeparator();

    mainMenu->addAction(m_toMultimediaPlayerAct);
    m_toMultimediaPlayerAct->setText("Multimedia Player");

    mainMenu->addAction(m_toPdfProcessorAct);
    m_toPdfProcessorAct->setText("PDF Processor");

    mainMenu->addAction(m_toCalenderAct);
    m_toCalenderAct->setText("Calender");

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