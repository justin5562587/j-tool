//
// Created by justin on 2020/11/19.
//
#include "MultimediaPlayer.h"
#include "PlayControl.h"
#include "VideoWidget.h"

#include <QVideoWidget>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStyle>

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : mainWidget(new QWidget) {
    mediaPlayer = new QMediaPlayer(mainWidget, QMediaPlayer::VideoSurface);

    // set VideoWidget
    QVideoWidget *videoWidget = new QVideoWidget(mainWidget);
    mediaPlayer->setVideoOutput(videoWidget);

    openBtn = new QPushButton("Open");
    connect(openBtn, &QAbstractButton::clicked, this, &MultimediaPlayer::openFile);

    errorLabel = new QLabel();
    errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);


    // set PlayControl
    PlayControl* playControl = new PlayControl(this);
    playControl->setState(m_player->state());
    playControl->setVolume(m_player->volume());
    playControl->setMuted(playControl->isMuted());

    connect(playControl, &PlayControl::play, m_player, &MultimediaPlayer::play);
//    connect(playControl, &PlayControl::pause, m_player, &QMediaPlayer::pause);
//    connect(playControl, &PlayControl::stop, m_player, &QMediaPlayer::stop);
//    connect(playControl, &PlayControl::next, m_playlist, &QMediaPlaylist::next);
//    connect(playControl, &PlayControl::previous, this, &Player::previousClicked);
//    connect(playControl, &PlayControl::changeVolume, m_player, &QMediaPlayer::setVolume);
//    connect(playControl, &PlayControl::changeMuting, m_player, &QMediaPlayer::setMuted);
//    connect(playControl, &PlayControl::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
//    connect(playControl, &PlayControl::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(playControl);
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
