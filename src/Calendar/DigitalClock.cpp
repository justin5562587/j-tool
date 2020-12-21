//
// Created by justin on 2020/12/21.
//

#include "DigitalClock.h"

#include <QTime>
#include <QTimer>

DigitalClock::DigitalClock(QWidget *parent) : QLCDNumber(parent) {
    setSegmentStyle(Filled);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DigitalClock::showTime);
    timer->start(1000);

    setDigitCount(19);
    showTime();

    setWindowTitle(tr("Digital Clock"));
//    resize(150, 60);
}

void DigitalClock::showTime() {
    QDateTime dateTime = QDateTime::currentDateTime();
    QString text = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    display(text);
}
