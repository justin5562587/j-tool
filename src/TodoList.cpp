//
// Created by justin on 2020/11/13.
//
#include "../include/TodoList.h"

#include <QHBoxLayout>
#include <QAction>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>

TodoList::TodoList(QWidget *parent) : todoListBox(new QGroupBox), todoItemsBox(new QScrollArea), todoItemsLayout(new QVBoxLayout),
                                      addTodoItemDialog(new AddTodoItemDialog), todoListData(new QVector<TodoItem>) {
    // right-bottom todolist box
    todoItemsBox->setLayout(todoItemsLayout);

    // right-top button box
    auto todoButtonLayout = new QHBoxLayout;
    auto *todoButtonBox = new QWidget;

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
    connect(addItemBtn, &QAbstractButton::clicked, this, &TodoList::addItem);

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
    auto rightLayout = new QVBoxLayout;
    auto rightBox = new QWidget;
    rightLayout->addWidget(todoButtonBox);
    rightLayout->addWidget(todoItemsBox);
    rightBox->setLayout(rightLayout);

    // left box
    auto leftBox = new QLabel;
    leftBox->setText("Welcome to TodoList");
//    QPixmap pixmap("../resources/test.jpeg");
//    leftBox->setPixmap(pixmap);

    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(leftBox);
    mainLayout->addWidget(rightBox);

    todoListBox->setLayout(mainLayout);
}

QGroupBox *TodoList::getSelfWidget() {
    return todoListBox;
}

// slots
void TodoList::clear() {
    todoListData->empty();
    refreshTodoList();
}

void TodoList::removeItem() {
//    todoListData->remove(i);
    refreshTodoList();
}

void TodoList::addItem() {
    addTodoItemDialog->show();

    if (addTodoItemDialog->exec()) {
        QString title = addTodoItemDialog->getTitle();
        QString description = addTodoItemDialog->getDescription();
        QString content = addTodoItemDialog->getContent();
        QDateTime startTime = addTodoItemDialog->getStartTime();
        QDateTime doneTime = addTodoItemDialog->getDoneTime();
        bool star = addTodoItemDialog->getStar();
        TodoItem todoItem = TodoItem(title, description, content, startTime, doneTime, star);
        todoListData->push_back(todoItem);

        refreshTodoList();
    }
}

void TodoList::refreshTodoList() {
    for (auto i = todoListData->begin(); i != todoListData->end(); ++i) {
        auto todoItemLabel = new QLabel;
        todoItemLabel->setText((*i).getTitle());
        todoItemsLayout->addWidget(todoItemLabel);
    }
}

void TodoList::loadFromFile() {}

void TodoList::saveToFile() {}
