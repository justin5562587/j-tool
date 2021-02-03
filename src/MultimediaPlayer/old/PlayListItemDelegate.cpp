//
// Created by justin on 2020/12/14.
//

#include "PlayListItemDelegate.h"

#include <QPainter>
#include <QSize>

PlayListItemDelegate::PlayListItemDelegate(QWidget *parent) : QStyledItemDelegate(parent) {
}

void PlayListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    int b = option.rect.bottom() - 1;

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(option.rect);

    if (index.row() < index.model()->rowCount() - 1) {
        painter->drawLine(option.rect.left() + 1, b, option.rect.right() - 1, b);
    }

    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}

QSize PlayListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QSize sz(QStyledItemDelegate::sizeHint(option, index));

    sz.setHeight(30);

    return sz;
}
