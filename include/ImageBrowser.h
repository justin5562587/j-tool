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

    void openImage();

    void closeImage();

    void zoomIn();

    void zoomOut();

private:

    void updateActions();

    QGroupBox *imageBrowserBox;
    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;

    QAction* openImageAct;
    QAction* closeImageAct;
    QAction* zoomInAct;
    QAction* zoomOutAct;

};
