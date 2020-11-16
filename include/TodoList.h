//
// Created by justin on 2020/11/13.
//

#pragma once

#include <QWidget>
#include <QVector>
#include <QGroupBox>
#include <QPushButton>
#include <QAction>

#include "../include/TodoItem.h"
#include "../include/AddTodoItemDialog.h"

class TodoList : public QWidget {
Q_OBJECT

public:
    TodoList(QWidget *parent = nullptr);

    QGroupBox* getSelfWidget();

public slots:

    void addItem();

private slots:

    void loadFromFile();

    void saveToFile();

//    void addItem();

    void showAddDialog();

    void removeItem();

    void clear();

private:

    QGroupBox* todoListBox;

    QVector<TodoItem>* todoListData;

    AddTodoItemDialog* addTodoItemDialog;

    QPushButton* loadFromFileBtn;
    QPushButton* saveToFileBtn;
    QPushButton* addItemBtn;
    QPushButton* clearBtn;
    QPushButton* removeItemBtn; // attached on TodoItem

    QAction *loadFromFileAct;
    QAction *saveToFileAct;
    QAction *addItemAct;
    QAction *clearAct;
    QAction *removeItemAct;

};