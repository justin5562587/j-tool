//
// Created by justin on 2020/11/03.
//

#pragma once

#include "../include/ImageBrowser.h"
#include "../include/TodoList.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QMenuBar>

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow();

private slots:

    // extra
    void about();

    void changeToImageBrowser();

    void changeToTodoList();

private:
    void createMenus();

    // inside widgets
    ImageBrowser imageBrowser;

    TodoList todoList;

    // actions

};
