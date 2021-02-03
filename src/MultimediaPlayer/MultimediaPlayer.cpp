//
// Created by justin on 2021/02/03.
//

#include <QBoxLayout>
#include <QSortFilterProxyModel>

#include "MultimediaPlayer.h"

MultimediaPlayer::MultimediaPlayer(QWidget *parent) : QWidget(parent) {
    // initialize playlist
    playListModel = new PlayListModel(this);
    playListView = new QListView(this);
    playListView->setModel(playListModel);

    // initialize screen && playControl
    playControl = new PlayControl(this);
    screen = new QLabel(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(screen);
    layout->addWidget(playControl);
    layout->addWidget(playListView);
}

MultimediaPlayer::~MultimediaPlayer() {

}
