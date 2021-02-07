//
// Created by justin on 2021/02/03.
//

#include "PlayControl.h"

#include <QStyle>
#include <QFileDialog>
#include <QStandardPaths>

PlayControl::PlayControl(QWidget *parent) : QWidget(parent) {
    // open layout
    openBtn = new QPushButton("Open", this);
    addUrlBtn = new QPushButton("Add", this);
    connect(addUrlBtn, &QAbstractButton::clicked, this, &PlayControl::addUrlFromInput);
    urlInput = new QLineEdit(this);
    urlInput->setPlaceholderText("Input path of local video file");
    QHBoxLayout *addLayout = new QHBoxLayout;
    addLayout->addWidget(openBtn);
    addLayout->addWidget(addUrlBtn);
    addLayout->addWidget(urlInput);

    // play layout
    playBtn = new QToolButton(this);
    playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    prevBtn = new QToolButton(this);
    prevBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    nextBtn = new QToolButton(this);
    nextBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    forwardBtn = new QToolButton(this);
    forwardBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    backwardBtn = new QToolButton(this);
    backwardBtn->setIcon(style()->standardIcon(QStyle::QStyle::SP_MediaSeekBackward));
    muteBtn = new QToolButton(this);
    muteBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    toPlaylistBtn = new QToolButton(this);
    toPlaylistBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));
    durationSlider = new QSlider(this);
    durationSlider->setOrientation(Qt::Horizontal);
    volumeSlider = new QSlider(this);
    volumeSlider->setOrientation(Qt::Horizontal);
    QHBoxLayout *playLayout = new QHBoxLayout;
    playLayout->addWidget(prevBtn);
    playLayout->addWidget(backwardBtn);
    playLayout->addWidget(playBtn);
    playLayout->addWidget(forwardBtn);
    playLayout->addWidget(nextBtn);
    playLayout->addWidget(durationSlider);
    playLayout->addWidget(muteBtn);
    playLayout->addWidget(volumeSlider);
    playLayout->addWidget(toPlaylistBtn);

    // advanced layout
    QHBoxLayout *advancedLayout = new QHBoxLayout;
    screenshotBtn = new QPushButton("Screenshot", this);
    advancedLayout->addWidget(screenshotBtn);

    connect(openBtn, &QAbstractButton::clicked, this, &PlayControl::open);
    connect(playBtn, &QAbstractButton::clicked, this, &PlayControl::play);

    // layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(addLayout);
    layout->addLayout(playLayout);
    layout->addLayout(advancedLayout);

    setLayout(layout);
}

PlayControl::~PlayControl() {

}

void PlayControl::open() {
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Video Files"));
    fileDialog.setDirectory(
            QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0, QDir::homePath())
    );
    if (fileDialog.exec() == QDialog::Accepted) {
        QList<QUrl> selectedUrls = fileDialog.selectedUrls();
        auto firstUrl = selectedUrls.begin();
        emit emitAddToPlayList(firstUrl->toString());
    }
}

void PlayControl::play() {
    const QString url = urlInput->text();
    emit emitPlay(url);
}

void PlayControl::addUrlFromInput() {
    const QString url = urlInput->text();
    emit emitAddToPlayList(url);
}
