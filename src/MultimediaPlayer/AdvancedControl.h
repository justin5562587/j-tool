//
// Created by justin on 2021/01/12.
//

#pragma once

#include <QMediaPlayer>
#include <QWidget>
#include <QAbstractButton>

enum AdvancedControlBtnTypes {
    VIDEO_RECORD,
    AUDIO_RECORD,
    SCREEN_SHOT,
    LOG_VIDEO_CODEC,
    LOG_AUDIO_CODEC
};

class AdvancedControl : public QWidget {
Q_OBJECT

public:

    explicit AdvancedControl(QWidget *parent = nullptr);

public slots:

    void active(QMediaPlayer *mediaPlayer);

    void inactive();

    void setBtnStatus(AdvancedControlBtnTypes advancedControlBtnTypes, bool isEnabled);

    void screenshot();

    void logVideoCodec();

    void logAudioCodec();

    void recordVideo();

    void recordAudio();

private:

    QMediaPlayer *m_mediaPlayer = nullptr;

    QAbstractButton *m_screenshotBtn = nullptr;
    QAbstractButton *m_videoCodecBtn = nullptr;
    QAbstractButton *m_audioCodecBtn = nullptr;
    QAbstractButton *m_videoRecordBtn = nullptr;
    QAbstractButton *m_audioRecordBtn = nullptr;

};