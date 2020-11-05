//
// Created by justin on 2020/11/05.
//
#pragma once

#include <QImage>
#include <QLabel>
#include <QScrollArea>

class ImageBrowser : public QWidget {
Q_OBJECT

public:
    ImageBrowser(QWidget *parent = nullptr);

    bool loadFile(const QString &);

private slots:

    void open();

    void print();

    void copy();

    void paste();

    void zoomIn();

    void zoomOut();

    void normalSize();

    void fitToWindow();

private:
    void createActions();

//    void createMenus();

    void updateActions();

//    bool saveFile(const QString &fileName);
//
//    void setImage(const QImage &newImage);

    void scaleImage(double factor);

    void adjustScrollBar(QScrollBar *scrollBar, double factor);

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
