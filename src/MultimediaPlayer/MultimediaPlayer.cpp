//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>
#include <QDebug>

#include "MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    fFmpegDecoder = FFmpegDecoder();

    playControl = new PlayControl(this);
    connect(playControl, &PlayControl::emitAddToPlayList, this, &MultimediaPlayer::addToPlayList);
    connect(playControl, &PlayControl::emitPlay, this, &MultimediaPlayer::play);

    screen = new QLabel(this);
    QPixmap pixmap("/Users/justin/Downloads/example_files/blackbuck.bmp");
    screen->setPixmap(pixmap);

    fFmpegDecoder.setScreen(screen);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(screen);
    layout->addWidget(playControl);
}

MultimediaPlayer::~MultimediaPlayer() {

}

void MultimediaPlayer::addToPlayList(const QString &url) {
    qInfo() << url;
    fileList.push_back(url);
}

void MultimediaPlayer::play(const QString &filename) {
    qInfo() << "Video File: " << filename;
    fFmpegDecoder.decodeVideo(filename.toStdString());
}
