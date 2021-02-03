//
// Created by justin on 2020/12/07.
//

#pragma once

#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QAbstractButton;

class QAbstractSlider;

class QComboBox;

QT_END_NAMESPACE

class PlayControl : public QWidget {
Q_OBJECT

public:

    explicit PlayControl(QWidget *parent = nullptr);

    QMediaPlayer::State state() const;
    int volume() const;
    bool isMuted() const;
    qreal playbackRate() const;

public slots:

    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMuted(bool muted);
    void setPlaybackRate(float rate);

signals:

    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeMuting(bool muting);
    void changeRate(qreal rate);

private slots:

    void playClicked();
    void mutedClicked();
    void updateRate();
    void onVolumeSliderValueChanged();

private:
    QMediaPlayer::State playerState = QMediaPlayer::StoppedState;
    bool playerMuted = false;
    QAbstractButton* playBtn = nullptr;
    QAbstractButton* stopBtn = nullptr;
    QAbstractButton* nextBtn = nullptr;
    QAbstractButton* previousBtn = nullptr;
    QAbstractButton* muteBtn = nullptr;
    QAbstractSlider* volumeSlider = nullptr;
    QComboBox* rateBox = nullptr;

};
