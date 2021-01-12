//
// Created by justin on 2021/01/12.
//

#include "AdvancedControl.h"

#include <QPushButton>
#include <QBoxLayout>

AdvancedControl::AdvancedControl(QWidget *parent) : QWidget(parent) {
    m_screenshotBtn = new QPushButton("ScreenShot", this);
    connect(m_screenshotBtn, &QAbstractButton::clicked, this, &AdvancedControl::screenshot);
    m_videoCodecBtn = new QPushButton("Log Video Codec", this);
    connect(m_videoCodecBtn, &QAbstractButton::clicked, this, &AdvancedControl::logVideoCodec);
    m_audioCodecBtn = new QPushButton("Log Audio Codec", this);
    connect(m_audioCodecBtn, &QAbstractButton::clicked, this, &AdvancedControl::logAudioCodec);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_screenshotBtn);
    layout->addWidget(m_videoCodecBtn);
    layout->addWidget(m_audioCodecBtn);
    setLayout(layout);
}

void AdvancedControl::screenshot() {

}

void AdvancedControl::logVideoCodec() {}

void AdvancedControl::logAudioCodec() {}


