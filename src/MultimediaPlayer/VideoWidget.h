//
// Created by justin on 2020/12/08.
//
#pragma once

#include <QVideoWidget>

class VideoWidget : public QVideoWidget {
Q_OBJECT

public:

    explicit VideoWidget(QWidget *parent = nullptr);

protected:

    void keyPressEvent(QKeyEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:

};
