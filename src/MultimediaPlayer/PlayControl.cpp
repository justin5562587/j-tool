//
// Created by justin on 2021/02/03.
//

#include "PlayControl.h"

#include <QFileDialog>
#include <QStandardPaths>

PlayControl::PlayControl(QWidget *parent) : QWidget(parent) {
    // UrlInputLayout
    addUrlBtn = new QPushButton(this);
    connect(addUrlBtn, &QAbstractButton::clicked, this, &PlayControl::addUrlFromInput);
    urlInput = new QLineEdit(this);
    urlInput->setPlaceholderText("Input File Url");
    QHBoxLayout *urlInputLayout = new QHBoxLayout(this);
    urlInputLayout->addWidget(addUrlBtn);
    urlInputLayout->addWidget(urlInput);

    openBtn = new QPushButton("Open", this);
    playBtn = new QPushButton("Play", this);

    connect(openBtn, &QAbstractButton::clicked, this, &PlayControl::open);
    connect(playBtn, &QAbstractButton::clicked, this, &PlayControl::playClicked);

    // layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addLayout(urlInputLayout);
    layout->addWidget(openBtn);
    layout->addWidget(playBtn);

    setLayout(layout);
}

PlayControl::~PlayControl() {

}

void PlayControl::open() {
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = m_player->supportedMimeTypes();

    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
//        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(
            QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0, QDir::homePath())
//            QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath())
    );

    if (fileDialog.exec() == QDialog::Accepted) {
        addToPlaylist(fileDialog.selectedUrls());
    }
}

void PlayControl::playClicked() {

}

void PlayControl::addUrlFromInput() {
    const QString url = urlInput->text();
    // todo insert file url
}

