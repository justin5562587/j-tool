//
// Created by justin on 2020/11/16.
//
#include "../include/AddTodoItemDialog.h"
#include "../include/TodoList.h"
#include "../include/MainWindow.h"

#include <QDebug>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>

AddTodoItemDialog::AddTodoItemDialog(QWidget *parent) : titleInput(new QLineEdit), descriptionInput(new QTextEdit),
                                                        contentInput(new QTextEdit), startTimeEdit(new QDateTimeEdit),
                                                        doneTimeEdit(new QDateTimeEdit), starChecker(new QCheckBox),
                                                        okBtn(new QPushButton("OK")),
                                                        cancelBtn(new QPushButton("Cancel")) {
    QLabel *titleLabel = new QLabel("Title");
    QLabel *descriptionLabel = new QLabel("Description");
    QLabel *contentLabel = new QLabel("Content");
    QLabel *startLabel = new QLabel("Start Time");
    QLabel *doneLabel = new QLabel("Done Time");
    QLabel *starLabel = new QLabel("Mark Star");

    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->setColumnStretch(1, 2);

    inputLayout->addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    inputLayout->addWidget(titleInput, 0, 1, Qt::AlignLeft);

    inputLayout->addWidget(descriptionLabel, 1, 0, Qt::AlignLeft);
    inputLayout->addWidget(descriptionInput, 1, 1, Qt::AlignLeft);

    inputLayout->addWidget(contentLabel, 2, 0, Qt::AlignLeft);
    inputLayout->addWidget(contentInput, 2, 1, Qt::AlignLeft);

    inputLayout->addWidget(startLabel, 3, 0, Qt::AlignLeft);
    inputLayout->addWidget(startTimeEdit, 3, 1, Qt::AlignLeft);

    inputLayout->addWidget(doneLabel, 4, 0, Qt::AlignLeft);
    inputLayout->addWidget(doneTimeEdit, 4, 1, Qt::AlignLeft);

    inputLayout->addWidget(starLabel, 5, 0, Qt::AlignLeft);
    inputLayout->addWidget(starChecker, 5, 1, Qt::AlignLeft);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    inputLayout->addLayout(buttonLayout, 6, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(inputLayout);

    setLayout(mainLayout);

//    connect(okBtn, &QAbstractButton::clicked, &TodoList::addItem);
    connect(okBtn, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle("Add Todo Item");
}

QString AddTodoItemDialog::getTitle() const {
    return titleInput->text();
}

QString AddTodoItemDialog::getDescription() const {
    return descriptionInput->toPlainText();
}

QString AddTodoItemDialog::getContent() const {
    return contentInput->toPlainText();
}

QDateTime AddTodoItemDialog::getStartTime() const {
    return startTimeEdit->dateTime();
}

QDateTime AddTodoItemDialog::getDoneTime() const {
    return doneTimeEdit->dateTime();
}

bool AddTodoItemDialog::getStar() const {
    return starChecker->isChecked();
}
