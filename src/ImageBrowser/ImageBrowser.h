//
// Created by justin on 2020/11/05.
//
#pragma once

#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QPushButton>

class ImageBrowser : public QWidget {
Q_OBJECT

public:
    ImageBrowser(QWidget *parent = nullptr);

private slots:

    void openImage();

    void closeImage();

    void adjustScrollBar(QScrollBar *, double);

    // scale control
    void scaleImage(double);

    void zoomIn();

    void zoomOut();

    void normalize();

    void fitWindow();

private:

    double scaleFactor = 1;

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;

    QPushButton *openImageBtn;
    QPushButton *closeImageBtn;
    QPushButton *zoomInBtn;
    QPushButton *zoomOutBtn;
    QPushButton *normalizeBtn;
    QPushButton *fitWindowBtn;

    QAction *openImageAct;
    QAction *closeImageAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalizeAct;
    QAction *fitWindowAct;

};
