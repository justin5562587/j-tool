//
// Created by justin on 2020/11/19.
//
#pragma once

#include <QWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;
QT_END_NAMESPACE

class MultimediaPlayer : public QWidget {
Q_OBJECT

public:

    MultimediaPlayer(QWidget* parent = nullptr);

    MultimediaPlayer(const MultimediaPlayer &) = delete;

    MultimediaPlayer &operator=(const MultimediaPlayer &) = delete;

    ~MultimediaPlayer();

    void setUrl(const QUrl& url);

    QWidget* getSelfWidget();

public slots:

    void openFile();

    void play();

private slots:

    void mediaStateChanged(QMediaPlayer::State state);

    void positionChanged(qint64 position);

    void durationChanged(qint64 duration);

    void setPosition(int position);

    void handleError();

private:

    // main widget
    QWidget* mainWidget;

    QMediaPlayer* mediaPlayer;

    QPushButton* openBtn;
    QPushButton* playBtn;

    QSlider* positionSlider;

    QLabel* errorLabel;


};
