//
// Created by justin on 2020/11/03.
//
#pragma once

#include "./MultimediaPlayer/MultimediaPlayer.h"
#include "./Calendar/Calendar.h"
#include "./PDFProcessor/PDFMain.h"
#include "./ScreenShot/ScreenShot.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QSignalMapper>

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow();

private slots:

    void about();

    void setCentralWidgetWith(int stackedWidgetIndex);

    void renderInfoSections();

private:
    void initializeWidget();

    void initializeActions();

    void initializeMenus();

    void createMenusForMultimediaPlayer();

    QSignalMapper* m_signalMapper;

    QWidget *m_infoWrapper = nullptr;

    QStackedWidget *m_stackedWidget;
    MultimediaPlayer *m_multimediaPlayer = nullptr;
    Calendar* m_calendar = nullptr;
    PDFMain* m_pdfProcessor = nullptr;
    ScreenShot* m_screenShot = nullptr;

    QVector<QAction* >* m_toActVector;
};
