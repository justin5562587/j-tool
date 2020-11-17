//
// Created by justin on 2020/11/13.
//

#pragma once

#include <QWidget>
#include <QVector>
#include <QGroupBox>
#include <QPushButton>
#include <QAction>
#include <QListWidget>

#include "../include/TodoItem.h"
#include "../include/AddTodoItemDialog.h"

class TodoList : public QWidget {
Q_OBJECT

public:
    TodoList(QWidget *parent = nullptr);

    QWidget *getSelfWidget();

private slots:

    void addItem();

    void removeItem();

    void refreshTodoList();

    void clear();

private:

    // main layout && widget
    QWidget* mainWidget;

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
