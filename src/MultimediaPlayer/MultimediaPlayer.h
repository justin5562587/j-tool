//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_MULTIMEDIAPLAYER_H
#define J_TOOL_MULTIMEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QMediaPlaylist>

#include "./FFmpegDecoder.h"
#include "./FFmpegRecorder.h"
#include "./PlayControl.h"
#include "./RecordControl.h"
#include "./PlaylistModel.h"

class MultimediaPlayer : public QWidget {
Q_OBJECT

public:

    explicit MultimediaPlayer(QWidget *parent = nullptr);

    ~MultimediaPlayer();

public slots:

    void addToPlayList(const QString &url);

    void play(const QString &filename);

    void jump(const QModelIndex &index);

    void recordVideo();

    void recordAudio();


private:

    RecordControl *recordControl;

    QLabel *screen;
    PlayControl *playControl;

    QListView *playlistView;
    PlaylistModel *playlistModel = nullptr;
    QMediaPlaylist *playlist = nullptr;

    FFmpegDecoder ffmpegDecoder;
    FFmpegRecorder ffmpegRecorder;
};

#endif //J_TOOL_MULTIMEDIAPLAYER_H
