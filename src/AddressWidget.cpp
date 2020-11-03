//
// Created by justin on 2020/11/03.
//
#include "../include/AddressWidget.h"

AddressWidget::AddressWidget(QWidget *parent) : QTabWidget(parent), table(new TableModel(this)), newAddressTab(new AddressTab(this)) {
    connect(newAddressTab, &NewAddressTab::sendDetails, this, &AddressWidget::addEntry);

    addTab(newAddressTab, tr("Address Book"));

    setupTabs();
}
