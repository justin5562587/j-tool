//
// Created by justin on 2021/02/03.
//

#include "PlayListModel.h"

PlayListModel::PlayListModel(QObject *parent) : QAbstractItemModel(parent) {
    fileLists = QList<FileInfo>();
}

QVariant PlayListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= fileLists.size()) {
        return QVariant();
    }

    QVariant ret;
    ret.setValue(fileLists.at(index.row()));
    return ret;
}

int PlayListModel::rowCount(const QModelIndex &parent) const {
    return fileLists.size();
}

bool PlayListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid()) {
        int row = index.row();
        FileInfo data = fileLists.at(row);
    }

    return true;
}

