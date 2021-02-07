//
// Created by justin on 2021/02/07.
//

#ifndef J_TOOL_RECORDCONTROL_H
#define J_TOOL_RECORDCONTROL_H

#include <QWidget>
#include <QAbstractButton>
#include <QPushButton>
#include <QBoxLayout>

class RecordControl : public QWidget {
Q_OBJECT
public:

    explicit RecordControl(QWidget *parent = nullptr);

    ~RecordControl();

public slots:

    void recordAudio();

    void recordVideo();

signals:

    void emitRecordAudio();

    void emitRecordVideo();

private:

    QAbstractButton *recordAudioBtn;
    QAbstractButton *recordVideoBtn;

};

#endif //J_TOOL_RECORDCONTROL_H
