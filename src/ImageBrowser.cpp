//
// Created by justin on 2020/11/05.
//
#include "../include/ImageBrowser.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QAction>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>

ImageBrowser::ImageBrowser(QWidget *parent) : imageBrowserBox(new QGroupBox), imageLabel(new QLabel), scrollArea(new QScrollArea) {
    QHBoxLayout* layout = new QHBoxLayout;

    openImageAct = new QAction;
    closeImageAct = new QAction;
    zoomInAct = new QAction;
    zoomOutAct = new QAction;

    QPushButton* openImageBtn = new QPushButton("Open Image");
    QPushButton* closeImageBtn = new QPushButton("Close Image");
    QPushButton* zoomInBtn = new QPushButton("Zoom In");
    QPushButton* zoomOutBtn = new QPushButton("Zoom Out");

    openImageBtn->addAction(openImageAct);
    connect(openImageAct, &QAction::triggered, this, &ImageBrowser::openImage);

    closeImageBtn->addAction(closeImageAct);
    connect(closeImageAct, &QAction::triggered, this, &ImageBrowser::closeImage);

    zoomInBtn->addAction(zoomInAct);
    connect(zoomInAct, &QAction::triggered, this, &ImageBrowser::zoomIn);

    zoomOutBtn->addAction(zoomOutAct);
    connect(zoomOutAct, &QAction::triggered, this, &ImageBrowser::zoomOut);

    layout->addWidget(openImageBtn);
    layout->addWidget(closeImageBtn);
    layout->addWidget(zoomInBtn);
    layout->addWidget(zoomOutBtn);
    imageBrowserBox->setLayout(layout);
}

// slots
void ImageBrowser::openImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Image"), "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    QImageReader reader(fileName);
    image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(this, "Image Load Information", tr("Cannot load %1: %2")
                .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return;
    }
    const QPixmap pix = QPixmap::fromImage(image);
    imageLabel->setPixmap(pix);
    // todo
}

void ImageBrowser::closeImage() {}

void ImageBrowser::zoomIn() {}

void ImageBrowser::zoomOut() {}
