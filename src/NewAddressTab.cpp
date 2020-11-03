//
// Created by justin on 2020/11/03.
//
#include "../include/NewAddressTab.h"
#include "../include/AddDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

NewAddressTab::NewAddressTab(QWidget *parent)
        : QWidget(parent)
{
    auto descriptionLabel = new QLabel(tr("There are currently no contacts in your address book. "
                                          "\nClick Add to add new contacts."));

    auto addButton = new QPushButton(tr("Add"));

    connect(addButton, &QAbstractButton::clicked, this, &NewAddressTab::addEntry);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(addButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
}

void NewAddressTab::addEntry() {
    AddDialog aDialog;

    if (aDialog.exec()) {
        emit sendDetails(aDialog.name(), aDialog.address());
    }
}


