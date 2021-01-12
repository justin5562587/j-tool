//
// Created by justin on 2021/01/12.
//

#pragma once

#include <QWidget>
#include <QAbstractButton>

class AdvancedControl : public QWidget {
Q_OBJECT

public:

    explicit AdvancedControl(QWidget *parent = nullptr);

public slots:

    void screenshot();

    void logVideoCodec();

    void logAudioCodec();

private:

    QAbstractButton *m_screenshotBtn = nullptr;
    QAbstractButton *m_videoCodecBtn = nullptr;
    QAbstractButton *m_audioCodecBtn = nullptr;

};