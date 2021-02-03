//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>
#include <QDebug>

#include "MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    // initialize screen && playControl
    playControl = new PlayControl(this);
    connect(playControl, &PlayControl::emitAddToPlayList, this, &MultimediaPlayer::addToPlayList);

    screen = new QLabel(this);
    screen->setText("screen");
    QPixmap pixmap("/Users/justin/Downloads/1111.jpg");
    screen->setPixmap(pixmap);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(screen);
    layout->addWidget(playControl);
}

MultimediaPlayer::~MultimediaPlayer() {

}

void MultimediaPlayer::addToPlayList(const QString &url) {
    qInfo() << url;
    fileList.push_back(url);
}
