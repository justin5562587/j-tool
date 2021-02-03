//
// Created by justin on 2020/12/14.
//

#pragma once

#include <QStyledItemDelegate>

class PlayListItemDelegate : public QStyledItemDelegate {
Q_OBJECT

public:

    PlayListItemDelegate(QWidget *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:

};
