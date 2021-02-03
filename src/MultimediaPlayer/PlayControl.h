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

    QAbstractButton *addUrlBtn;
    QLineEdit *urlInput;
    QAbstractButton *openBtn;
    QAbstractButton *playBtn;

};


#endif //J_TOOL_PLAYCONTROL_H
