
//
// Created by justin on 2020/12/10.
//
#pragma once

#include <QWidget>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QLabel>

class MediaInfoWidget : public QWidget {

Q_OBJECT

public:

    explicit MediaInfoWidget(QWidget *parent);

    void populateWidgets(QMediaPlayer* mediaPlayer, QVector<QString>& keys);

    void clearWidgets();

private:

    QVBoxLayout* mainLayout;

    QLabel* emptyLabel = nullptr;

    QVector<QString> defaultKeys = std::initializer_list<QString>({ "Title" });

};
