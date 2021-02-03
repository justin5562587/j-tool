//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>

#include "MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    playControl = new PlayControl(this);
    screen = new QLabel(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(screen);
    layout->addWidget(playControl);
}

MultimediaPlayer::~MultimediaPlayer() {

}
