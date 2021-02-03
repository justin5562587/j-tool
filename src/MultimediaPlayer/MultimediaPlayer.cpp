//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>

#include "MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    // initialize playlist
//    playListModel = new PlayListModel(this);
//    playListView = new QListView(this);
//    playListView->setModel(playListModel);
//    playListModel->insertRow(playListModel->rowCount());

    // initialize screen && playControl
    playControl = new PlayControl(this);
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
