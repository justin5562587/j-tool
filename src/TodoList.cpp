//
// Created by justin on 2020/11/13.
//
#include "../include/TodoList.h"

#include <QHBoxLayout>
#include <QAction>
#include <QLabel>
#include <QScrollArea>

TodoList::TodoList(QWidget *parent) : todoListBox(new QGroupBox), addTodoItemDialog(new AddTodoItemDialog) {
    // right-bottom todolist box
    QVBoxLayout *todoItemsLayout = new QVBoxLayout;
    QScrollArea *todoItemsBox = new QScrollArea;
    todoItemsBox->setLayout(todoItemsLayout);

    // right-top button box
    QHBoxLayout *todoButtonLayout = new QHBoxLayout;
    QWidget *todoButtonBox = new QWidget;

    loadFromFileAct = new QAction;
    saveToFileAct = new QAction;
    addItemAct = new QAction;
    removeItemAct = new QAction;
    clearAct = new QAction;

    addItemBtn = new QPushButton("Add");
    loadFromFileBtn = new QPushButton("Load");
    saveToFileBtn = new QPushButton("Save");
    clearBtn = new QPushButton("Clear");

    addItemBtn->addAction(addItemAct);
    connect(addItemBtn, &QAbstractButton::clicked, this, &TodoList::showAddDialog);

    loadFromFileBtn->addAction(loadFromFileAct);
    connect(loadFromFileBtn, &QAbstractButton::clicked, this, &TodoList::loadFromFile);

    saveToFileBtn->addAction(saveToFileAct);
    connect(saveToFileBtn, &QAbstractButton::clicked, this, &TodoList::saveToFile);

    clearBtn->addAction(clearAct);
    connect(clearBtn, &QAbstractButton::clicked, this, &TodoList::clear);

    todoButtonLayout->addWidget(addItemBtn);
    todoButtonLayout->addWidget(loadFromFileBtn);
    todoButtonLayout->addWidget(saveToFileBtn);
    todoButtonLayout->addWidget(clearBtn);
    todoButtonBox->setLayout(todoButtonLayout);

    // right box
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QWidget *rightBox = new QWidget;
    rightLayout->addWidget(todoButtonBox);
    rightLayout->addWidget(todoItemsBox);
    rightBox->setLayout(rightLayout);

    // left box
    QLabel *leftBox = new QLabel;
    leftBox->setText("Welcome to TodoList");
//    QPixmap pixmap("../resources/test.jpeg");
//    leftBox->setPixmap(pixmap);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(leftBox);
    mainLayout->addWidget(rightBox);

    todoListBox->setLayout(mainLayout);
}

QGroupBox *TodoList::getSelfWidget() {
    return todoListBox;
}

// slots
void TodoList::clear() {
    todoListData->clear();
}

void TodoList::removeItem() {
//    todoListData->remove(i);
}

void TodoList::showAddDialog() {
    addTodoItemDialog->show();
}

void TodoList::addItem() {
    QString title = addTodoItemDialog->getTitle();
    QString description = addTodoItemDialog->getDescription();
    QString content = addTodoItemDialog->getContent();
    QDateTime startTime = addTodoItemDialog->getStartTime();
    QDateTime doneTime = addTodoItemDialog->getDoneTime();
    bool star = addTodoItemDialog->getStar();
    TodoItem todoItem = TodoItem(title, description, content, startTime, doneTime, star);
    todoListData->push_back(todoItem);
}

void TodoList::loadFromFile() {}

void TodoList::saveToFile() {

}
