//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>
#include <QDebug>

#include "MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    fFmpegDecoder = FFmpegDecoder();

    playlist = new QMediaPlaylist();
    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
    playlistView = new QListView;
    playlistView->setModel(playlistModel);
    connect(playlistView, &QAbstractItemView::activated, this, &MultimediaPlayer::jump);

    playControl = new PlayControl;
    connect(playControl, &PlayControl::emitAddToPlayList, this, &MultimediaPlayer::addToPlayList);
    connect(playControl, &PlayControl::emitPlay, this, &MultimediaPlayer::play);

    // displayLayout
    screen = new QLabel;
    screen->setStyleSheet("QLabel { background-color : black; }");
    fFmpegDecoder.setScreen(screen);

    QGridLayout *displayLayout = new QGridLayout;
    displayLayout->addWidget(screen, 0, 0, 1, 2);
    displayLayout->addWidget(playlistView, 0,1,1,2);

    // main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(displayLayout);
    layout->addWidget(playControl);
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
    qInfo() << "Video File: " << filename;
    fFmpegDecoder.decodeMultimediaFile(filename.toStdString());
}
