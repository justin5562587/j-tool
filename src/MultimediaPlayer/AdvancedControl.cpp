//
// Created by justin on 2021/01/12.
//

#include "AdvancedControl.h"
#include "./frameProcessor.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QDebug>

AdvancedControl::AdvancedControl(QWidget *parent) : QWidget(parent) {
    m_screenshotBtn = new QPushButton("ScreenShot", this);
    connect(m_screenshotBtn, &QAbstractButton::clicked, this, &AdvancedControl::screenshot);
    m_videoCodecBtn = new QPushButton("Log Video Codec", this);
    connect(m_videoCodecBtn, &QAbstractButton::clicked, this, &AdvancedControl::logVideoCodec);
    m_audioCodecBtn = new QPushButton("Log Audio Codec", this);
    connect(m_audioCodecBtn, &QAbstractButton::clicked, this, &AdvancedControl::logAudioCodec);

    inactive();

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_screenshotBtn);
    layout->addWidget(m_videoCodecBtn);
    layout->addWidget(m_audioCodecBtn);
    setLayout(layout);
}

void AdvancedControl::active(QMediaPlaylist *mediaPlaylist) {
    m_mediaPlaylist = mediaPlaylist;
    m_screenshotBtn->setEnabled(true);
    m_videoCodecBtn->setEnabled(true);
    m_audioCodecBtn->setEnabled(true);
}

void AdvancedControl::inactive() {
    m_mediaPlaylist = nullptr;
    m_screenshotBtn->setEnabled(false);
    m_videoCodecBtn->setEnabled(false);
    m_audioCodecBtn->setEnabled(false);
}

void AdvancedControl::screenshot() {
    QMediaContent mediaContent = m_mediaPlaylist->currentMedia();
    std::string urlString = mediaContent.request().url().toString().toStdString();
    getPixmapWithTimestamp(urlString.substr(7), 5);
}

void AdvancedControl::logVideoCodec() {
    QMediaContent mediaContent = m_mediaPlaylist->currentMedia();
    QUrl url = mediaContent.request().url();
    qInfo() << url.toString();
}

void AdvancedControl::logAudioCodec() {
    qInfo() << "log audio codec info";
}


