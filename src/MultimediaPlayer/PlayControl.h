//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_PLAYCONTROL_H
#define J_TOOL_PLAYCONTROL_H

#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>
#include <QSlider>
#include <QToolButton>

class PlayControl : public QWidget {
Q_OBJECT
public:

    explicit PlayControl(QWidget *parent = nullptr);

    ~PlayControl();

public slots:

    void open();

    void playClicked();

    void addUrlFromInput();

signals:

    void play();

    void pause();

private:

    QAbstractButton *openBtn;
    QAbstractButton *addUrlBtn;
    QLineEdit *urlInput;

    QToolButton *playBtn;
    QToolButton *prevBtn;
    QToolButton *nextBtn;
    QToolButton *forwardBtn;
    QToolButton *backwardBtn;
    QToolButton *muteBtn;
    QToolButton *toPlaylistBtn;
    QSlider *durationSlider;
    QSlider *volumeSlider;

    QAbstractButton *screenshotBtn;

};


#endif //J_TOOL_PLAYCONTROL_H
