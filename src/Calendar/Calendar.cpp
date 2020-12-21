//
// Created by justin on 2020/12/21.
//

#include "Calendar.h"

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QLabel>
#include <QLocale>
#include <QTextCharFormat>

Calendar::Calendar(QWidget *parent) : QWidget(parent) {
    createTimeWidget();
    createOptionsWidget();

    m_digitalClock = new DigitalClock(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_digitalClock);
    layout->addWidget(calendar);
    layout->addWidget(optionsWidget);
    setLayout(layout);

    setWindowTitle(tr("Calendar"));
}

void Calendar::localeChanged(int index) {
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

//! [1]
void Calendar::firstDayChanged(int index) {
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
            firstDayCombo->itemData(index).toInt()));
}

//! [2]
void Calendar::selectedDateChanged() {
    currentDateEdit->setDate(calendar->selectedDate());
}
//! [2]

//! [5]
void Calendar::weekdayFormatChanged() {
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
            weekdayColorCombo->itemData(weekdayColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Monday, format);
    calendar->setWeekdayTextFormat(Qt::Tuesday, format);
    calendar->setWeekdayTextFormat(Qt::Wednesday, format);
    calendar->setWeekdayTextFormat(Qt::Thursday, format);
    calendar->setWeekdayTextFormat(Qt::Friday, format);
}
//! [5]

//! [6]
void Calendar::weekendFormatChanged() {
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}

//! [8]
void Calendar::reformatCalendarPage() {
    QTextCharFormat mayFirstFormat;
    const QDate mayFirst(calendar->yearShown(), 5, 1);

    QTextCharFormat firstFridayFormat;
    QDate firstFriday(calendar->yearShown(), calendar->monthShown(), 1);
    while (firstFriday.dayOfWeek() != Qt::Friday) {
        firstFriday = firstFriday.addDays(1);
    }

    calendar->setDateTextFormat(mayFirst, mayFirstFormat);
}

void Calendar::createTimeWidget() {
    calendar = new QCalendarWidget;
    calendar->setFixedWidth(800);
    calendar->setFixedHeight(400);
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);

    connect(calendar, &QCalendarWidget::currentPageChanged,this, &Calendar::reformatCalendarPage);
}

void Calendar::createOptionsWidget() {
    optionsWidget = new QWidget(this);

    currentDateEdit = new QDateEdit;
    currentDateEdit->setDisplayFormat("yyyy MM dd");
    currentDateEdit->setDate(calendar->selectedDate());
    currentDateEdit->setDateRange(calendar->minimumDate(),calendar->maximumDate());

    QLabel* currentDateLabel = new QLabel(tr("&Current Date:"));
    currentDateLabel->setBuddy(currentDateEdit);

    connect(currentDateEdit, &QDateEdit::dateChanged, calendar, &QCalendarWidget::setSelectedDate);
    connect(calendar, &QCalendarWidget::selectionChanged,this, &Calendar::selectedDateChanged);

    localeCombo = new QComboBox;
    int curLocaleIndex = -1;
    int index = 0;
    for (int _lang = QLocale::C; _lang <= QLocale::LastLanguage; ++_lang) {
        QLocale::Language lang = static_cast<QLocale::Language>(_lang);
        QList<QLocale::Country> countries = QLocale::countriesForLanguage(lang);
        for (int i = 0; i < countries.count(); ++i) {
            QLocale::Country country = countries.at(i);
            QString label = QLocale::languageToString(lang);
            label += QLatin1Char('/');
            label += QLocale::countryToString(country);
            QLocale locale(lang, country);
            if (this->locale().language() == lang && this->locale().country() == country)
                curLocaleIndex = index;
            localeCombo->addItem(label, locale);
            ++index;
        }
    }
    if (curLocaleIndex != -1) {
        localeCombo->setCurrentIndex(curLocaleIndex);
    }
    QLabel* localeLabel= new QLabel(tr("&Locale"));
    localeLabel->setBuddy(localeCombo);

    firstDayCombo = new QComboBox;
    firstDayCombo->addItem(tr("Sunday"), Qt::Sunday);
    firstDayCombo->addItem(tr("Monday"), Qt::Monday);

    QLabel* firstDayLabel = new QLabel(tr("Wee&k starts on:"));
    firstDayLabel->setBuddy(firstDayCombo);

    gridCheckBox = new QCheckBox(tr("&Grid"));
    gridCheckBox->setChecked(calendar->isGridVisible());

    navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
    navigationCheckBox->setChecked(true);

    connect(localeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Calendar::localeChanged);
    connect(firstDayCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Calendar::firstDayChanged);
    connect(gridCheckBox, &QCheckBox::toggled, calendar, &QCalendarWidget::setGridVisible);
    connect(navigationCheckBox, &QCheckBox::toggled, calendar, &QCalendarWidget::setNavigationBarVisible);

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(gridCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(navigationCheckBox);

    weekdayColorCombo = createColorComboBox();
    weekdayColorCombo->setCurrentIndex(weekdayColorCombo->findText(tr("Black")));

    QLabel* weekdayColorLabel = new QLabel(tr("&Weekday color:"));
    weekdayColorLabel->setBuddy(weekdayColorCombo);

    weekendColorCombo = createColorComboBox();
    weekendColorCombo->setCurrentIndex(weekendColorCombo->findText(tr("Red")));

    QLabel* weekendColorLabel = new QLabel(tr("Week&end color:"));
    weekendColorLabel->setBuddy(weekendColorCombo);

    connect(weekdayColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Calendar::weekdayFormatChanged);
    connect(weekdayColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Calendar::reformatCalendarPage);
    connect(weekendColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Calendar::weekendFormatChanged);
    connect(weekendColorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &Calendar::reformatCalendarPage);

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(currentDateLabel, 0, 0);
    outerLayout->addWidget(currentDateEdit, 0 ,1);
    outerLayout->addWidget(localeLabel, 1, 0);
    outerLayout->addWidget(localeCombo, 1, 1);
    outerLayout->addWidget(firstDayLabel, 2, 0);
    outerLayout->addWidget(firstDayCombo, 2, 1);
    outerLayout->addWidget(weekdayColorLabel, 3, 0);
    outerLayout->addWidget(weekdayColorCombo, 3, 1);
    outerLayout->addWidget(weekendColorLabel, 4, 0);
    outerLayout->addWidget(weekendColorCombo, 4, 1);
    outerLayout->addLayout(checkBoxLayout, 5, 0, 1, 2);

    weekdayFormatChanged();
    weekendFormatChanged();
    reformatCalendarPage();

    optionsWidget->setLayout(outerLayout);
}

QComboBox *Calendar::createColorComboBox() {
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("Red"), QColor(Qt::red));
    comboBox->addItem(tr("Blue"), QColor(Qt::blue));
    comboBox->addItem(tr("Black"), QColor(Qt::black));
    comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
    return comboBox;
}
