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

    // extra
    void about();

    void setWithImageBrowser();

    void setWithTodoList();

    void setWithMultimediaPlayer();

private:
    void createMenus();

    QWidget* centralWidget;

    QVBoxLayout* mainLayout;

    // inside widgets
    ImageBrowser imageBrowser;
    QWidget *imageBrowserWidget;

    TodoList todoList;
    QWidget *todoListWidget;

    MultimediaPlayer multimediaPlayer;
    QWidget* multimediaPlayerWidget;
};
