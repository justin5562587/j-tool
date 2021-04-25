//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>
#include <QVector>

#include "./MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) :
        QWidget(parent),
        cacheController("/Users/justin/cpp/j-tool/src/MultimediaPlayer/j_multimedia_player_cache.txt") {

    qInfo() << "MultimediaPlayer thread ID: " << pthread_self();

    // initialize core components
    cacheController.loadCache();

    // initialize ffmpeg components
    ffmpegDecoder = FFmpegDecoder();
    ffmpegRecorder = FFmpegRecorder();

    // displayLayout
    playlist = new QMediaPlaylist();
    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
    playlistView = new QListView;
    playlistView->setModel(playlistModel);
    connect(playlistView, &QAbstractItemView::activated, this, &MultimediaPlayer::jump);

    screen = new QLabel;
    screen->setStyleSheet("QLabel { background-color : black; }");
    ffmpegDecoder.setScreen(screen);

    QBoxLayout *displayLayout = new QHBoxLayout();
    displayLayout->addWidget(screen, 2);
    displayLayout->addWidget(playlistView);

    // main layout
    playControl = new PlayControl;
    connect(playControl, &PlayControl::emitAddToPlayList, this, &MultimediaPlayer::addToPlayList);
    connect(playControl, &PlayControl::emitPlay, this, &MultimediaPlayer::play);
    connect(playControl, &PlayControl::emitStop, this, &MultimediaPlayer::stop);

    recordControl = new RecordControl;
    connect(recordControl, &RecordControl::emitRecordVideo, this, &MultimediaPlayer::recordVideo);
    connect(recordControl, &RecordControl::emitRecordAudio, this, &MultimediaPlayer::recordAudio);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(displayLayout);
    layout->addWidget(playControl);
    layout->addWidget(recordControl);
}

MultimediaPlayer::~MultimediaPlayer() {

}

void MultimediaPlayer::addToPlayList(const QString &filepath) {
    QUrl url = QUrl(filepath);
    playlist->addMedia(url);
}

void MultimediaPlayer::jump(const QModelIndex &index) {
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        QUrl currentFileUrl = playlist->currentMedia().request().url();
        play(currentFileUrl.toString());
    }
}

void MultimediaPlayer::play(const QString &filename) {
    ffmpegDecoder.decodeMultimediaFile(filename.toStdString());
}

void MultimediaPlayer::stop() {
    ffmpegDecoder.stopDecode();
}

void MultimediaPlayer::recordAudio() {
//    ffmpegRecorder.doRecord(AUDIO);
    cacheController.writeCache("2012-12-09 04:11:39 av_information.mp4");
}

void MultimediaPlayer::recordVideo() {
    if (isRecording == 1) {
        recordControl->changeRecordVideoBtnStatus(false);
        isRecording = -1;
    } else {
        recordControl->changeRecordVideoBtnStatus(true);
        isRecording = 1;
    }
    ffmpegRecorder.doRecord(VIDEO);
}
