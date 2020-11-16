//
// Created by justin on 2020/11/16.
//

#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QString>
#include <QPushButton>

class AddTodoItemDialog : public QDialog {
Q_OBJECT

public:

    AddTodoItemDialog(QWidget *parent = nullptr);

    QString getTitle() const;

    QString getDescription() const;

    QString getContent() const;

    QDateTime getStartTime() const;

    QDateTime getDoneTime() const;

    bool getStar() const;

private slots:

private:

    QLineEdit *titleInput;
    QTextEdit *descriptionInput;
    QTextEdit *contentInput;
    QDateTimeEdit *startTimeEdit;
    QDateTimeEdit *doneTimeEdit;
    QCheckBox *starChecker;

    QPushButton* okBtn;
    QPushButton* cancelBtn;

};
