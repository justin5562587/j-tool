//
// Created by justin on 2020/12/15.
//

#pragma once

#include <QPixmap>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;
QT_END_NAMESPACE

class ScreenShot : public QWidget {

Q_OBJECT

public:
    explicit ScreenShot(QWidget* parent);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void newScreenShot();
    void saveScreenShot();
    void shootScreen();
    void updateCheckBox();

private:
    void updateScreenshotLabel();

    QPixmap originalPixmap;

    QLabel *screenshotLabel;
    QSpinBox *delaySpinBox;
    QCheckBox *hideThisWindowCheckBox;
    QPushButton *newScreenshotButton;
};
