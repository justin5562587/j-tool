//
// Created by justin on 2020/11/13.
//

#pragma once

#include "./TodoItem.h"
#include "./AddTodoItemDialog.h"

#include <QWidget>
#include <QVector>
#include <QGroupBox>
#include <QPushButton>
#include <QAction>
#include <QListWidget>

class TodoList : public QWidget {
Q_OBJECT

public:
    TodoList(QWidget *parent = nullptr);

private slots:

    void addItem();

    void removeItem();

    void refreshTodoList();

    void clear();

private:

    QListView* pendingBox = nullptr;
    QListView* doneBox = nullptr;

    // data vector
    QVector<TodoItem> *pendingTodoVector;
    QVector<TodoItem> *doneTodoVector;

    AddTodoItemDialog *addTodoItemDialog;

    // button && actions
    QAction *addItemAct;
    QAction *clearAct;
    QAction *removeItemAct;

};
