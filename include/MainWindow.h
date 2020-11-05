//
// Created by justin on 2020/11/03.
//

#pragma once

#include "../include/AddressWidget.h"

#include <QMainWindow>
#include <QItemSelection>

#include <QMenuBar>
//
//class MainWindow : public QMainWindow {
//Q_OBJECT
//
//public:
//    MainWindow();
//
//private slots:
//    void updateActions(const QItemSelection& selection);
//
//    void openFile();
//
//    void saveFile();
//
//private:
//    void createMenus();
//
//    AddressWidget* addressWidget;
//    QAction *editAct;
//    QAction *removeAct;
//
//};

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
    void selectImageAndShow();
    void selectFolderAndShow();


private:
    void createContactMenus();
    void createImageBrowserMenus();

    // Contact Menu
    QAction* showContactMenuAct;
    QAction* saveContactMenuAct;
    QAction* loadContactMenuAct;

    // Image Browser
    QAction* selectImageAct;
    QAction* selectImageFolderAct;

};
