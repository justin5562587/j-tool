//
// Created by justin on 2021/01/12.
//

#include "AdvancedControl.h"
#include "./ffmpegFrame.h"
#include "./ffmpegResample.h"

#include <QPushButton>
#include <QStandardPaths>
#include <QBoxLayout>

AdvancedControl::AdvancedControl(QWidget *parent) : QWidget(parent) {
    m_screenshotBtn = new QPushButton("ScreenShot", this);
    connect(m_screenshotBtn, &QAbstractButton::clicked, this, &AdvancedControl::screenshot);
    m_videoCodecBtn = new QPushButton("Log Video Codec", this);
    connect(m_videoCodecBtn, &QAbstractButton::clicked, this, &AdvancedControl::logVideoCodec);
    m_audioCodecBtn = new QPushButton("Log Audio Codec", this);
    connect(m_audioCodecBtn, &QAbstractButton::clicked, this, &AdvancedControl::logAudioCodec);
    m_videoRecordBtn = new QPushButton("Record Video", this);
    connect(m_videoRecordBtn, &QAbstractButton::clicked, this, &AdvancedControl::recordVideo);
    m_audioRecordBtn = new QPushButton("Record Audio", this);
    connect(m_audioRecordBtn, &QAbstractButton::clicked, this, &AdvancedControl::recordAudio);

    inactive();

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_screenshotBtn);
    layout->addWidget(m_videoCodecBtn);
    layout->addWidget(m_audioCodecBtn);
    layout->addWidget(m_videoRecordBtn);
    layout->addWidget(m_audioRecordBtn);
    setLayout(layout);
}

void AdvancedControl::active(QMediaPlayer *mediaPlayer) {
    m_mediaPlayer = mediaPlayer;
    m_screenshotBtn->setEnabled(true);
    m_videoCodecBtn->setEnabled(true);
    m_audioCodecBtn->setEnabled(true);
}

void AdvancedControl::inactive() {
    m_mediaPlayer = nullptr;
    m_screenshotBtn->setEnabled(false);
    m_videoCodecBtn->setEnabled(false);
    m_audioCodecBtn->setEnabled(false);
}

// todo - optimize precision of screenshot
void AdvancedControl::screenshot() {
    QMediaContent mediaContent = m_mediaPlayer->currentMedia();
    std::string urlString = mediaContent.request().url().toString().toStdString();

    double currentSeconds = (double) m_mediaPlayer->position() / 1000;
    QString downloadDir = QStandardPaths::locate(QStandardPaths::DownloadLocation, "", QStandardPaths::LocateDirectory);

    getPixmapInSpecificSeconds(urlString.substr(7), currentSeconds, downloadDir.toStdString());
}

void AdvancedControl::logVideoCodec() {
    QMediaContent mediaContent = m_mediaPlayer->currentMedia();
    QUrl url = mediaContent.request().url();
    qInfo() << url.toString();
}

void AdvancedControl::logAudioCodec() {
    qInfo() << "log audio codec info";
}

void AdvancedControl::recordVideo() {

}

void AdvancedControl::recordAudio() {
    recordAudioWithFFmpeg();
}

