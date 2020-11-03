//
// Created by justin on 2020/11/03.
//

#pragma once

#include <QTabWidget>
#include <QWidget>
#include <QString>
#include <QItemSelection>

#include "../include/TableModel.h"
#include "../include/NewAddressTab.h"

class AddressWidget : public QTabWidget {
Q_OBJECT

public:

    AddressWidget(QWidget *parent = nullptr);

    void readFromFile(const QString &filename);

    void writeToFile(const QString &filename);

public slots:

    void showAddEntryDialog();

    void addEntry(const QString &name, const QString &address);

    void editEntry();

    void removeEntry();

signals:
    void selectionChanged(const QItemSelection& selected);

private:
    void setupTabs();

    TableModel* table;
    NewAddressTab *newAddressTab;

};
