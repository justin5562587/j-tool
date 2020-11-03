//
// Created by justin on 2020/11/03.
//

#pragma once

#include "../include/AddressWidget.h"

#include <QMainWindow>
#include <QItemSelection>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow();

private slots:
    void updateActions(const QItemSelection& selection);

    void openFile();

    void saveFile();

private:
    void createMenus();

    AddressWidget* addressWidget;
    QAction *editAct;
    QAction *removeAct;

};