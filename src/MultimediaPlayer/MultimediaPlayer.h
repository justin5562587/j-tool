//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_MULTIMEDIAPLAYER_H
#define J_TOOL_MULTIMEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QListView>

#include "./PlayControl.h"

class MultimediaPlayer : public QWidget {
Q_OBJECT

public:

    explicit MultimediaPlayer(QWidget *parent = nullptr);

    ~MultimediaPlayer();

private:

    QVector<QString> fileList;

    QLabel *screen;

    PlayControl *playControl;
};


#endif //J_TOOL_MULTIMEDIAPLAYER_H
