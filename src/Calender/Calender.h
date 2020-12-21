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
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;
QT_END_NAMESPACE

class Calender : public QWidget
{
Q_OBJECT

public:
    Calender(QWidget *parent = nullptr);

private slots:
    void localeChanged(int index);
    void firstDayChanged(int index);
    void selectedDateChanged();
    void weekdayFormatChanged();
    void weekendFormatChanged();
    void reformatCalendarPage();

private:
    void createPreviewGroupBox();
    void createGeneralOptionsGroupBox();
    void createTextFormatsGroupBox();
    QComboBox *createColorComboBox();

    QCalendarWidget *calendar;

    QGroupBox *generalOptionsGroupBox;
    QLabel *currentDateLabel;
    QDateEdit *currentDateEdit;
    QLabel *localeLabel;
    QLabel *firstDayLabel;

    QComboBox *localeCombo;
    QComboBox *firstDayCombo;
    QCheckBox *gridCheckBox;
    QCheckBox *navigationCheckBox;

    QGroupBox *textFormatsGroupBox;
    QLabel *weekdayColorLabel;
    QLabel *weekendColorLabel;
    QComboBox *weekdayColorCombo;
    QComboBox *weekendColorCombo;

    DigitalClock* m_digitalClock = nullptr;
};