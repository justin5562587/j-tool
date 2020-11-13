//
// Created by justin on 2020/11/13.
//
#include "../include/TodoList.h"

#include <QHBoxLayout>
#include <QLabel>

TodoList::TodoList(QWidget *parent) : todoListBox(new QGroupBox) {
    QHBoxLayout* mainLayout = new QHBoxLayout;

    QLabel* titleLabel = new QLabel("Todo list");
    mainLayout->addWidget(titleLabel);
    todoListBox->setLayout(mainLayout);
}

QGroupBox* TodoList::getSelfWidget() {
    return todoListBox;
}

// slots
void TodoList::clear() {
    todoListData->clear();
}

void TodoList::removeItem(int i) {
    todoListData->remove(i);
}

void TodoList::addItem(TodoItem& todoItem) {
    todoListData->push_back(todoItem);
}

void TodoList::loadFromFile() {}

void TodoList::saveToFile() {

}


