//
// Created by justin on 2020/12/07.
//
#include "PlayControl.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QAudio>

PlayControl::PlayControl(QWidget *parent) : QWidget(parent) {
    playBtn = new QToolButton(this);
    playBtn->setShortcut(QKeySequence(Qt::Key_Space));
    playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playBtn, &QAbstractButton::clicked, this, &PlayControl::playClicked);

    stopBtn = new QToolButton(this);
    stopBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    connect(stopBtn, &QAbstractButton::clicked, this, &PlayControl::stop);

    nextBtn = new QToolButton(this);
    nextBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextBtn, &QAbstractButton::clicked, this, &PlayControl::next);

    previousBtn = new QToolButton(this);
    previousBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(previousBtn, &QAbstractButton::clicked, this, &PlayControl::previous);

    muteBtn = new QToolButton(this);
    muteBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    connect(muteBtn, &QAbstractButton::clicked, this, &PlayControl::mutedClicked);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    connect(volumeSlider, &QAbstractSlider::valueChanged, this, &PlayControl::onVolumeSliderValueChanged);

    rateBox = new QComboBox(this);
    rateBox->addItem("0.5x", QVariant(0.5));
    rateBox->addItem("1.0x", QVariant(1.0));
    rateBox->addItem("2.0x", QVariant(2.0));
    rateBox->setCurrentIndex(1);
    connect(rateBox, QOverload<int>::of(&QComboBox::activated), this, &PlayControl::updateRate);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(stopBtn);
    layout->addWidget(previousBtn);
    layout->addWidget(playBtn);
    layout->addWidget(nextBtn);
    layout->addWidget(muteBtn);
    layout->addWidget(volumeSlider);
    layout->addWidget(rateBox);

    setLayout(layout);
}

QMediaPlayer::State PlayControl::state() const {
    return playerState;
}

void PlayControl::setState(QMediaPlayer::State state) {
    if (state != playerState) {
        playerState = state;
        switch (state) {
            case QMediaPlayer::StoppedState:
                stopBtn->setEnabled(false);
                playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                break;
            case QMediaPlayer::PlayingState:
                stopBtn->setEnabled(true);
                playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
                break;
            case QMediaPlayer::PausedState:
                stopBtn->setEnabled(true);
                playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                break;
        }
    }
}

int PlayControl::volume() const {
    qreal linearVolume = QAudio::convertVolume(volumeSlider->value() / qreal(100),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    return qRound(linearVolume * 100);
}

void PlayControl::setVolume(int volume) {
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);
    volumeSlider->setValue(qRound(logarithmicVolume * 100));
}

bool PlayControl::isMuted() const {
    return playerMuted;
}

void PlayControl::setMuted(bool muted) {
    if (muted != playerMuted) {
        playerMuted = muted;
        muteBtn->setIcon(style()->standardIcon(muted
                                                    ? QStyle::SP_MediaVolumeMuted
                                                    : QStyle::SP_MediaVolume));
    }
}

void PlayControl::playClicked() {
    switch (playerState) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            emit play();
            break;
        case QMediaPlayer::PlayingState:
            emit pause();
            break;
    }
}

void PlayControl::mutedClicked() {
    emit changeMuting(!playerMuted);
}

qreal PlayControl::playbackRate() const {
    return rateBox->itemData(rateBox->currentIndex()).toDouble();
}

void PlayControl::setPlaybackRate(float rate) {
    for (int i = 0; i < rateBox->count(); ++i) {
        if (qFuzzyCompare(rate, float(rateBox->itemData(i).toDouble()))) {
            rateBox->setCurrentIndex(i);
            return;
        }
    }

    rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    rateBox->setCurrentIndex(rateBox->count() - 1);
}

void PlayControl::updateRate() {
    emit changeRate(playbackRate());
}

void PlayControl::onVolumeSliderValueChanged() {
    emit changeVolume(volume());
}
