//
// Created by justin on 2021/01/12.
//

#include "AdvancedControl.h"

#include <QPushButton>
#include <QStandardPaths>
#include <QBoxLayout>

AdvancedControl::AdvancedControl(QWidget *parent) : QWidget(parent) {
    m_ffmpegFrame = FFmpegFrame();
    m_ffmpegRecord = FFmpegRecord();

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

void AdvancedControl::setBtnStatus(AdvancedControlBtnTypes advancedControlBtnTypes, bool isEnabled) {
    switch (advancedControlBtnTypes) {
        case VIDEO_RECORD:
            m_videoRecordBtn->setEnabled(isEnabled);
            m_videoRecordBtn->setText(isEnabled ? "Record Video" : "Recording Video...");
            break;
        case AUDIO_RECORD:
            m_audioRecordBtn->setEnabled(isEnabled);
            m_audioRecordBtn->setText(isEnabled ? "Record Audio" : "Recording Audio...");
            break;
        case SCREEN_SHOT:
        case LOG_VIDEO_CODEC:
        case LOG_AUDIO_CODEC:
        default:
            break;
    }
}

// todo - optimize precision of screenshot
void AdvancedControl::screenshot() {
    QMediaContent mediaContent = m_mediaPlayer->currentMedia();
    std::string urlString = mediaContent.request().url().toString().toStdString();

    double currentSeconds = (double) m_mediaPlayer->position() / 1000;
    QString downloadDir = QStandardPaths::locate(QStandardPaths::DownloadLocation, "", QStandardPaths::LocateDirectory);

    m_ffmpegFrame.saveFrameImage(urlString.substr(7), currentSeconds, AV_PIX_FMT_RGB24, downloadDir.toStdString());
}

void AdvancedControl::logVideoCodec() {
    QMediaContent mediaContent = m_mediaPlayer->currentMedia();
    QUrl url = mediaContent.request().url();
    qInfo() << url.toString();
}

void AdvancedControl::logAudioCodec() {
    qInfo() << "\nAdvancedControl::logAudioCodec";
}

void AdvancedControl::recordVideo() {
//    this->setBtnStatus(VIDEO_RECORD, false);
//    recordVideoWithFFmpeg(this, "/User/justin/Downloads/");
    m_ffmpegRecord.recordVideo(AV_CODEC_ID_H264, "FFmpegRecord_output.mp4");
}

void AdvancedControl::recordAudio() {
    qInfo() << "\nAdvancedControl::recordAudio";
//    this->setBtnStatus(AUDIO_RECORD, false);
//    recordAudioWithFFmpeg(this, "/User/justin/Downloads/");
}
