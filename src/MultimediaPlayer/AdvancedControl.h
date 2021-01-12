//
// Created by justin on 2021/01/12.
//

#pragma once

#include <QMediaPlaylist>
#include <QWidget>
#include <QAbstractButton>

class AdvancedControl : public QWidget {
Q_OBJECT

public:

    explicit AdvancedControl(QWidget *parent = nullptr);

public slots:

    void active(QMediaPlaylist *mediaPlaylist);

    void inactive();

    void screenshot();

    void logVideoCodec();

    void logAudioCodec();

private:

    QMediaPlaylist *m_mediaPlaylist = nullptr;

    QAbstractButton *m_screenshotBtn = nullptr;
    QAbstractButton *m_videoCodecBtn = nullptr;
    QAbstractButton *m_audioCodecBtn = nullptr;

};