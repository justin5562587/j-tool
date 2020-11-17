//
// Created by justin on 2020/11/03.
//

#pragma once

#include "../include/ImageBrowser.h"
#include "../include/TodoList.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QMenuBar>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow();

private slots:

    // extra
    void about();

    void setWithImageBrowser();

    void setWithTodoList();

private:
    void createMenus();

    QWidget* centralWidget;

    QVBoxLayout* mainLayout;

    // inside widgets
    ImageBrowser imageBrowser;
    QGroupBox *imageBrowserWidget;

    TodoList todoList;
    QGroupBox *todoListWidget;
};
