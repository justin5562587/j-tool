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
class QAbstractItemView;

class QLabel;

class QMediaPlayer;

class QModelIndex;

class QPushButton;

class QSlider;

class QStatusBar;

class QVideoProbe;

class QVideoWidget;

class QAudioProbe;

QT_END_NAMESPACE

class PlaylistModel;

class HistogramWidget;

class MultimediaPlayer : public QWidget {
Q_OBJECT

public:

    explicit MultimediaPlayer(QWidget *parent = nullptr);

    MultimediaPlayer(const MultimediaPlayer &) = delete;

    MultimediaPlayer &operator=(const MultimediaPlayer &) = delete;

    ~MultimediaPlayer();

    bool isPlayerAvailable() const;

    void addToPlaylist(const QList<QUrl> &urls);

    void setCustomAudioRole(const QString &role);

signals:

    void fullScreenChanged(bool fullScreen);

private slots:
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();
    void previousClicked();
    void seek(int seconds);
    void jump(const QModelIndex& index);
    void playlistPositionChanged(int);
    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);
    void displayErrorMessage();
    void showColorDialog();

private:

    void clearHistogram();

    void setTrackInfo(const QString &info);

    void setStatusInfo(const QString &info);

    void handleCursor(QMediaPlayer::MediaStatus status);

    void updateDurationInfo(qint64 currentInfo);

    QMediaPlayer *m_player = nullptr;
    QMediaPlaylist *m_playlist = nullptr;
    QVideoWidget *m_videoWidget = nullptr;
    QLabel *m_coverLabel = nullptr;
    QSlider *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;
    QPushButton *m_fullScreenButton = nullptr;
    QPushButton *m_colorButton = nullptr;
    QDialog *m_colorDialog = nullptr;
    QLabel *m_statusLabel = nullptr;
    QStatusBar *m_statusBar = nullptr;

    QLabel *m_labelHistogram = nullptr;
    HistogramWidget *m_videoHistogram = nullptr;
    HistogramWidget *m_audioHistogram = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    QAudioProbe *m_audioProbe = nullptr;

    PlaylistModel *m_playlistModel = nullptr;
    QAbstractItemView *m_playlistView = nullptr;
    QString m_trackInfo;
    QString m_statusInfo;
    qint64 m_duration;


};
