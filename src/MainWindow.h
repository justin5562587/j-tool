//
// Created by justin on 2020/11/03.
//
#pragma once

#include "./ImageBrowser/ImageBrowser.h"
#include "./TodoList/TodoList.h"
#include "./MultimediaPlayer/MultimediaPlayer.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QMenuBar>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow();

private slots:

    void about();

    void setCentralWithImageBrowser();

    void setCentralWithTodoList();

    void setCentralWithMultimediaPlayer();

private:
    void createMenus();

    ImageBrowser* m_imageBrowser = nullptr;
    TodoList* m_todoList = nullptr;
    MultimediaPlayer* m_multimediaPlayer = nullptr;
};
