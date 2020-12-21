//
// Created by justin on 2020/12/21.
//

#pragma once

#include <QLCDNumber>

class DigitalClock : public QLCDNumber {
Q_OBJECT

public:
    DigitalClock(QWidget *parent = nullptr);

private slots:

    void showTime();
};
