//
// Created by justin on 2020/11/19.
//
#include "MultimediaPlayer.h"

#include <QVideoWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStyle>

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : mainWidget(new QWidget) {
    mediaPlayer = new QMediaPlayer(mainWidget, QMediaPlayer::VideoSurface);

    QVideoWidget *videoWidget = new QVideoWidget;

    openBtn = new QPushButton("Open");
    connect(openBtn, &QAbstractButton::clicked, this, &MultimediaPlayer::openFile);

    playBtn = new QPushButton("Play");
    playBtn->setEnabled(false);
    connect(playBtn, &QAbstractButton::clicked, this, &MultimediaPlayer::play);

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);
    connect(positionSlider, &QAbstractSlider::sliderMoved, this, &MultimediaPlayer::setPosition);

    errorLabel = new QLabel();
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openBtn);
    controlLayout->addWidget(playBtn);
    controlLayout->addWidget(positionSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoWidget);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(errorLabel);

    mediaPlayer->setVideoOutput(videoWidget);
    connect(mediaPlayer, &QMediaPlayer::stateChanged, this, &MultimediaPlayer::mediaStateChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MultimediaPlayer::positionChanged);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MultimediaPlayer::durationChanged);
    connect(mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this,&MultimediaPlayer::handleError);

    mainWidget->setLayout(mainLayout);
}

MultimediaPlayer::~MultimediaPlayer() {
}

QWidget* MultimediaPlayer::getSelfWidget() {
    return mainWidget;
}

void MultimediaPlayer::openFile() {
    QFileDialog fileDialog(mainWidget);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle("Select Video");
    QStringList supportedMimeTypes = mediaPlayer->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(
            QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted) {
        setUrl(fileDialog.selectedUrls().constFirst());
    }
}

void MultimediaPlayer::setUrl(const QUrl &url) {
    errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mediaPlayer->setMedia(url);
    playBtn->setEnabled(true);
}

void MultimediaPlayer::play() {
    switch (mediaPlayer->state()) {
        case QMediaPlayer::PlayingState:
            mediaPlayer->pause();
            break;
        default:
            mediaPlayer->play();
            break;
    }
}

void MultimediaPlayer::mediaStateChanged(QMediaPlayer::State state) {
    switch (state) {
        case QMediaPlayer::PlayingState:
            playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
    }
}

void MultimediaPlayer::positionChanged(qint64 position) {
    positionSlider->setValue(position);
}

void MultimediaPlayer::durationChanged(qint64 duration) {
    positionSlider->setRange(0, duration);
}

void MultimediaPlayer::setPosition(int position) {
    mediaPlayer->setPosition(position);
}

void MultimediaPlayer::handleError() {
    playBtn->setEnabled(false);
    const QString errorString = mediaPlayer->errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(mediaPlayer->error()));
    else
        message += errorString;
    errorLabel->setText(message);
}
