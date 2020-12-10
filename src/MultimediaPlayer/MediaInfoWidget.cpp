//
// Created by justin on 2020/12/10.
//

#include "MediaInfoWidget.h"

#include <QLabel>

MediaInfoWidget::MediaInfoWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);

    QLabel* emptyLabel = new QLabel("No Media Focused", this);
    mainLayout->addWidget(emptyLabel);

    setLayout(mainLayout);
}

void MediaInfoWidget::populateWidget(QMediaPlayer *mediaPlayer, QVector<QString> &keys) {
    keys.append(defaultKeys);
    auto begin = keys.begin();
    auto end = keys.end();
    while (begin != end) {
        QString dataOfKey = *begin;
        QVariant item =  mediaPlayer->metaData(dataOfKey);
        QLabel* label = new QLabel(dataOfKey);

        // get value according to keys
        label->setText(item.toString());

        mainLayout->addWidget(label);
        ++begin;
    }
}
