//
// Created by justin on 2020/11/03.
//
#include "../include/AddressWidget.h"
#include "../include/NewAddressTab.h"
#include "../include/AddDialog.h"

#include <QSortFilterProxyModel>
#include <QTableView>
#include <QMessageBox>
#include <QHeaderView>
#include <QFile>

AddressWidget::AddressWidget(QWidget *parent) : QTabWidget(parent), table(new TableModel(this)),
                                                newAddressTab(new NewAddressTab(this)) {
    connect(newAddressTab, &NewAddressTab::sendDetails, this, &AddressWidget::addEntry);
    addTab(newAddressTab, tr("Address Book"));
    setupTabs();
}

void AddressWidget::setupTabs() {
    const auto groups = {"ABC", "DEF", "GHI", "JKL", "MNO", "PQR", "STU", "VW", "XYZ"};

    for (const QString &str : groups) {
        const auto regExp = QRegularExpression(QString("^[%1].*").arg(str),
                                               QRegularExpression::CaseInsensitiveOption);

        auto proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(table);
        proxyModel->setFilterRegularExpression(regExp);
        proxyModel->setFilterKeyColumn(0);

        QTableView *tableView = new QTableView;
        tableView->setModel(proxyModel);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->hide();
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSortingEnabled(true);

        connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &AddressWidget::selectionChanged);

        connect(this, &QTabWidget::currentChanged, this, [this, tableView](int tabIndex) {
            if (widget(tabIndex) == tableView)
                    emit selectionChanged(tableView->selectionModel()->selection());
        });

        addTab(tableView, str);
    }
}

void AddressWidget::showAddEntryDialog() {
    AddDialog aDialog;

    if (aDialog.exec()) {
        addEntry(aDialog.name(), aDialog.address());
    }
}

void AddressWidget::addEntry(const QString &name, const QString &address) {
    if (!table->getContacts().contains({name, address})) {
        table->insertRows(0, 1, QModelIndex());

        QModelIndex index = table->index(0, 0, QModelIndex());
        table->setData(index, name, Qt::EditRole);
        index = table->index(0, 1, QModelIndex());
        table->setData(index, address, Qt::EditRole);
        removeTab(indexOf(newAddressTab));
    } else {
        QMessageBox::information(this, tr("Duplicate Name"),
                                 tr("The name \"%1\" already exists.").arg(name));
    }
}

void AddressWidget::editEntry() {
    QTableView *temp = static_cast<QTableView *>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel *>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    const QModelIndexList indexes = selectionModel->selectedRows();
    QString name;
    QString address;
    int row = -1;

    for (const QModelIndex &index : indexes) {
        row = proxy->mapToSource(index).row();
        QModelIndex nameIndex = table->index(row, 0, QModelIndex());
        QVariant varName = table->data(nameIndex, Qt::DisplayRole);
        name = varName.toString();

        QModelIndex addressIndex = table->index(row, 1, QModelIndex());
        QVariant varAddr = table->data(addressIndex, Qt::DisplayRole);
        address = varAddr.toString();
    }

    AddDialog aDialog;
    aDialog.setWindowTitle(tr("Edit a Contact"));
    aDialog.editAddress(name, address);

    if (aDialog.exec()) {
        const QString newAddress = aDialog.address();
        if (newAddress != address) {
            const QModelIndex index = table->index(row, 1, QModelIndex());
            table->setData(index, newAddress, Qt::EditRole);
        }
    }
}

void AddressWidget::removeEntry() {
    QTableView *temp = static_cast<QTableView *>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel *>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    const QModelIndexList indexes = selectionModel->selectedRows();

    for (QModelIndex index : indexes) {
        int row = proxy->mapToSource(index).row();
        table->removeRows(row, 1, QModelIndex());
    }

    if (table->rowCount(QModelIndex()) == 0)
        insertTab(0, newAddressTab, tr("Address Book"));
}

void AddressWidget::writeToFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QDataStream out(&file);
    out << table->getContacts();
}

void AddressWidget::readFromFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file.errorString());
        return;
    }

    QVector<Contact> contacts;
    QDataStream in(&file);
    in >> contacts;

    if (contacts.isEmpty()) {
        QMessageBox::information(this, tr("No contacts in file"),
                                 tr("The file you are attempting to open contains no contacts."));
    } else {
        for (const auto &contact: qAsConst(contacts))
            addEntry(contact.name, contact.address);
    }
}