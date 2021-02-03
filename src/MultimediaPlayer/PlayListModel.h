//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_PLAYLISTMODEL_H
#define J_TOOL_PLAYLISTMODEL_H

#include <QAbstractListModel>

typedef struct FileInfo {
    QString filename;
    QString url;
    QString type;
    QString size;
    QString duration;
} FileInfo;
Q_DECLARE_METATYPE(FileInfo)

class PlayListModel : public QAbstractListModel {
Q_OBJECT

public:

    explicit PlayListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

private:

    QList<FileInfo> fileLists;
};


#endif //J_TOOL_PLAYLISTMODEL_H
