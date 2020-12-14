//
// Created by justin on 2020/11/13.
//
#include "TodoList.h"

#include <QHBoxLayout>
#include <QAction>
#include <QLabel>
#include <QButtonGroup>
#include <QFile>
//#include <>
#include <QDebug>


TodoList::TodoList(QWidget *parent) : QWidget(parent),
                                      pendingTodoVector(new QVector<TodoItem>), doneTodoVector(new QVector<TodoItem>),
                                      addTodoItemDialog(new AddTodoItemDialog) {

    auto mainLayout = new QVBoxLayout;

    // top section (title)
    QLabel *titleLabel = new QLabel("Todo List", this);
    titleLabel->setStyleSheet("font-size: 20pt");

    // middle section (pendingBox doneBox)
    QHBoxLayout *middleLayout = new QHBoxLayout;

    QVBoxLayout *pendingLayout = new QVBoxLayout;
    pendingBox = new QListView;
    pendingBox->setStyleSheet("QListView { font-size: 20pt; font-weight: bold; }"
                              "QListView::item { background-color: #2ECC71; padding: 10%;"
                              "border: 1px solid #27AE60; }"
                              "QListView::item::hover { background-color: #27AE60 }");
    pendingBox->setLayout(pendingLayout);

    QVBoxLayout *doneLayout = new QVBoxLayout;
    doneBox = new QListView;
    doneBox->setStyleSheet("QListView { font-size: 20pt; font-weight: bold; }"
                           "QListView::item { background-color: #2ECC71; padding: 10%;"
                           "border: 1px solid #27AE60; }"
                           "QListView::item::hover { background-color: #27AE60 }");
    doneBox->setLayout(doneLayout);

    middleLayout->addWidget(pendingBox);
    middleLayout->addWidget(doneBox);

    // bottom section (todoButtonBox)
    QHBoxLayout *bottomLayout = new QHBoxLayout;

    QPushButton *addBtn = new QPushButton("Add");
    addBtn->setIcon(QIcon(":/icons/add.png"));
    addBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    QPushButton *removeBtn = new QPushButton("Remove");
    removeBtn->setIcon(QIcon(":/icons/remove.png"));

    bottomLayout->addWidget(addBtn);
    bottomLayout->addWidget(removeBtn);

    addItemAct = new QAction;
    removeItemAct = new QAction;

    addBtn->addAction(addItemAct);
    connect(addBtn, &QAbstractButton::clicked, this, &TodoList::addItem);

    removeBtn->addAction(removeItemAct);
    connect(removeBtn, &QAbstractButton::clicked, this, &TodoList::removeItem);

    QLabel* imageLabel = new QLabel;
    QPixmap pix(":/resources/images/1.jpg");
    imageLabel->setPixmap(pix);
//    imageLabel->setText("test label");

    //  set mainLayout
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addWidget(imageLabel);

    // load specific style
    QFile styleFile(":/resources/qss/todoList.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    setStyleSheet(styleSheet);

    setLayout(mainLayout);
}

// slots
void TodoList::clear() {
//    todoListData->clear();
//    refreshTodoList();
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
        pendingTodoVector->push_back(todoItem);

        refreshTodoList();
    }
}

void TodoList::refreshTodoList() {
    for (auto i = pendingTodoVector->begin(); i != pendingTodoVector->end(); ++i) {
//        auto todoItemLabel = new QLabel;
//        todoItemLabel->setText((*i).getTitle());
//        pendingTodoVector->addItem((*i).getTitle());
    }
}
