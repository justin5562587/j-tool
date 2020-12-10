
//
// Created by justin on 2020/12/10.
//
#pragma once

#include <QWidget>
#include <QMediaPlayer>
#include <QVBoxLayout>

class MediaInfoWidget : public QWidget {

Q_OBJECT

public:

    explicit MediaInfoWidget(QWidget *parent);

    void populateWidget(QMediaPlayer* mediaPlayer, QVector<QString>& keys);

private:

    QVBoxLayout* mainLayout;

    QVector<QString> defaultKeys = std::initializer_list<QString>({ "Title" });

};
