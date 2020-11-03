//
// Created by justin on 2020/11/03.
//

#pragma once

#include <QWidget>
#include <QString>

class NewAddressTab : public QWidget {
Q_OBJECT

public:

    NewAddressTab(QWidget *parent = nullptr);

public slots:
    void addEntry();

signals:
    void sendDetails(const QString& name, const QString& address);

};

