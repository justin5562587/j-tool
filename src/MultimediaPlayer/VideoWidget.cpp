//
// Created by justin on 2020/12/08.
//
#include "VideoWidget.h"

#include <QKeyEvent>

VideoWidget::VideoWidget(QWidget *parent) : QVideoWidget(parent) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

//    QPalette p = palette();
//    p.setColor(QPalette::Window, Qt::blue);
//    setPalette(p);
    setObjectName("CustomVideoWidget");
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void VideoWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        setFullScreen(false);
        event->accept();
    } else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
        setFullScreen(!isFullScreen());
        event->accept();
    } else {
        QVideoWidget::keyPressEvent(event);
    }
}

void VideoWidget::mousePressEvent(QMouseEvent *event) {
//    setFullScreen(!isFullScreen());
    event->accept();
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    setFullScreen(!isFullScreen());
    QVideoWidget::mousePressEvent(event);
}
