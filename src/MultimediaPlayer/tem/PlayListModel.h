//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_PLAYLISTMODEL_H
#define J_TOOL_PLAYLISTMODEL_H

#include <QAbstractItemModel>

class PlayListModel : public QAbstractItemModel {
Q_OBJECT

public:

    explicit PlayListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:

    QMap<QModelIndex, QVariant> data;
};


#endif //J_TOOL_PLAYLISTMODEL_H
