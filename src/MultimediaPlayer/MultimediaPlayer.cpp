//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>
#include <QVector>
#include <SDL_thread.h>
#include <SDL_timer.h>

#include "./MultimediaPlayer.h"

static int testThread(void *ptr)
{
    int cnt;

    for (cnt = 0; cnt < 50; ++cnt) {
        printf("Thread counter: %d\n", cnt);
        SDL_Delay(200);
    }

    return cnt;
}

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    qInfo() << "MultimediaPlayer thread ID: " << pthread_self();

    int threadReturnValue;
    SDL_Thread *sdlThread = SDL_CreateThread(testThread, "TestThread", nullptr);
    if (NULL == threadReturnValue) {
        printf("SDL_CreateThread failed: %s\n", SDL_GetError());
    } else {
        SDL_WaitThread(sdlThread, &threadReturnValue);
        printf("Thread returned value: %d\n", threadReturnValue);
    }

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
    ffmpegRecorder.record(AUDIO);
}

void MultimediaPlayer::recordVideo() {
     ffmpegRecorder.record(VIDEO);
}


