//
// Created by justin on 2020/11/05.
//
#pragma once

#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>

class ImageBrowser : public QWidget {
Q_OBJECT

public:
    ImageBrowser(QWidget *parent = nullptr);

    bool loadFile(const QString &);

private slots:

    void open();

    void print();

    void zoomIn();

    void zoomOut();

    void normalSize();

    void fitToWindow();

private:

    void createActions();

    void createMenus();

    void updateActions();

    void scaleImage(double factor);

    void adjustScrollBar(QScrollBar *scrollBar, double factor);


    QGroupBox *imageBrowserBox;

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printer;
#endif

    QAction *printAct;
    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

};
