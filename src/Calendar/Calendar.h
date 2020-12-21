//
// Created by justin on 2020/12/21.
//
#pragma once

#include "DigitalClock.h"

#include <QWidget>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDateEdit;
class QGridLayout;
class QLabel;
QT_END_NAMESPACE

class Calendar : public QWidget
{
Q_OBJECT

public:
    Calendar(QWidget *parent = nullptr);

private slots:
    void localeChanged(int index);
    void firstDayChanged(int index);
    void selectedDateChanged();
    void weekdayFormatChanged();
    void weekendFormatChanged();
    void reformatCalendarPage();

private:
    void createTimeWidget();
    void createOptionsWidget();
    QComboBox *createColorComboBox();

    QCalendarWidget *calendar;

    QWidget *optionsWidget;
    QDateEdit *currentDateEdit;

    QComboBox *localeCombo;
    QComboBox *firstDayCombo;
    QCheckBox *gridCheckBox;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;
    QCheckBox *navigationCheckBox;

    DigitalClock* m_digitalClock = nullptr;
};