//
// Created by justin on 2021/02/03.
//

#include "PlayControl.h"

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

}

void PlayControl::playClicked() {

}

void PlayControl::addUrlFromInput() {
    const QString url = urlInput->text();
    // todo insert file url
}

