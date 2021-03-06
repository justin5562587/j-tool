//
// Created by justin on 2020/11/05.
//
#include "ImageBrowser.h"

#include <QHBoxLayout>
#include <QAction>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QScrollBar>

ImageBrowser::ImageBrowser(QWidget *parent) : QWidget(parent), imageLabel(new QLabel), scrollArea(new QScrollArea) {
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setScaledContents(true);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    openImageAct = new QAction;
    closeImageAct = new QAction;
    zoomInAct = new QAction;
    zoomOutAct = new QAction;
    normalizeAct = new QAction;
    fitWindowAct = new QAction;

    openImageBtn = new QPushButton("Open Image");
    closeImageBtn = new QPushButton("Close Image");
    zoomInBtn = new QPushButton("Zoom In");
    zoomOutBtn = new QPushButton("Zoom Out");
    normalizeBtn = new QPushButton("Normalize");
    fitWindowBtn = new QPushButton("Fit Window");

    openImageBtn->addAction(openImageAct);
    connect(openImageBtn, &QAbstractButton::clicked, this, &ImageBrowser::openImage);

    closeImageBtn->addAction(closeImageAct);
    connect(closeImageBtn, &QAbstractButton::clicked, this, &ImageBrowser::closeImage);

    zoomInBtn->addAction(zoomInAct);
    connect(zoomInBtn, &QAbstractButton::clicked, this, &ImageBrowser::zoomIn);

    zoomOutBtn->addAction(zoomOutAct);
    connect(zoomOutBtn, &QAbstractButton::clicked, this, &ImageBrowser::zoomOut);
    
    normalizeBtn->addAction(normalizeAct);
    connect(normalizeBtn, &QAbstractButton::clicked, this, &ImageBrowser::normalize);

    fitWindowBtn->addAction(fitWindowAct);
    connect(fitWindowBtn, &QAbstractButton::clicked, this, &ImageBrowser::fitWindow);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(openImageBtn);
    buttonLayout->addWidget(closeImageBtn);
    buttonLayout->addWidget(zoomInBtn);
    buttonLayout->addWidget(zoomOutBtn);
    buttonLayout->addWidget(normalizeBtn);
    buttonLayout->addWidget(fitWindowBtn);

    QHBoxLayout* contentLayout = new QHBoxLayout;
    scrollArea->setWidget(imageLabel);
    scrollArea->setBackgroundRole(QPalette::Dark);
    contentLayout->addWidget(scrollArea);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(contentLayout);

    setLayout(mainLayout);
}

// slots
void ImageBrowser::openImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Image"), "",
                                                    tr("Images (*.png *.bmp *.jpg *.jpeg *.tif *.GIF);;All Files (*)"));
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
    scaleFactor = 1.0;
    scrollArea->setVisible(true);

    const QPixmap pix = QPixmap::fromImage(image);
    imageLabel->setPixmap(pix);

    if (!fitWindowAct->isChecked()) {
        imageLabel->adjustSize();
    }
}

void ImageBrowser::closeImage() {
    normalize();
    imageLabel->clear();
}

void ImageBrowser::adjustScrollBar(QScrollBar* scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

// scale control
void ImageBrowser::scaleImage(double factor) {
    scaleFactor*=factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageBrowser::zoomIn() {
    scaleImage(1.25);
}

void ImageBrowser::zoomOut() {
    scaleImage(0.8);
}

void ImageBrowser::normalize() {
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageBrowser::fitWindow() {
    bool fitToWindow = fitWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (fitToWindow) {
        normalize();
    }
}
