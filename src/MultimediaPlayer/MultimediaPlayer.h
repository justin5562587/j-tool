//
// Created by justin on 2020/11/19.
//
#pragma once

#include <QWidget>

class MultimediaPlayer : public QWidget {
Q_OBJECT

public:

    MultimediaPlayer();

    MultimediaPlayer(const MultimediaPlayer &) = delete;

    MultimediaPlayer &operator=(const MultimediaPlayer &) = delete;

private slots:

    void openFile();

private:

    // main widget
    QWidget* mainWidget;


};
