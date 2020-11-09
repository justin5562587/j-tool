//
// Created by justin on 2020/11/03.
//

#pragma once

#include "../include/ImageBrowser.h"

#include <QMainWindow>
#include <QItemSelection>

#include <QMenuBar>

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow();

    MainWindow(QWidget *parent);

private slots:
    // Contact Menu
    void showContactMenu();
    void saveContactMenu();
    void loadContactMenu();

    // Image Browser
    void openAndShowImage();

    // extra
    void about();

private:
    void createContactMenus();
    void createImageBrowserMenus();
    void createExtraMenus();

    // inside widgets
    ImageBrowser imageBrowser;

    // Contact Menu
    QAction* showContactMenuAct;
    QAction* saveContactMenuAct;
    QAction* loadContactMenuAct;

    // Image Browser
    QAction* openImageAct;

};
