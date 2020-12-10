//
// Created by justin on 2020/12/10.
//

#include "MediaInfoWidget.h"

#include <QLabel>

MediaInfoWidget::MediaInfoWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);

    emptyLabel = new QLabel("No Media Focused", this);
    mainLayout->addWidget(emptyLabel);

    setLayout(mainLayout);
}

void MediaInfoWidget::populateWidgets(QMediaPlayer *mediaPlayer, QVector <QString> &keys) {
    clearWidgets();

    keys.append(defaultKeys);
    auto begin = keys.begin();
    auto end = keys.end();
    while (begin != end) {
        QString dataOfKey = *begin;
        QVariant item = mediaPlayer->metaData(dataOfKey);
        QLabel * label = new QLabel(dataOfKey);

        // get value according to keys
//        label->setText(item.toString());

        qInfo() << "Current label counts is " + QString::number(mainLayout->count());

        mainLayout->addWidget(label);
        ++begin;
    }
}

// todo
void MediaInfoWidget::clearWidgets() {
//    for (auto i in range(layout.count())) {
//        layout.itemAt(i).widget().setParent(None);
//    }
//    for i in reversed(range(layout.count())):

}
