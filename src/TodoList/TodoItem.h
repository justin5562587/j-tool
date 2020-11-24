//
// Created by justin on 2020/11/13.
//

#pragma once

#include <QString>
#include <QDateTime>

class TodoItem {
public:
    const QString &getTitle() const;

    void setTitle(const QString &title);

    const QString &getDescription() const;

    void setDescription(const QString &description);

    const QString &getContent() const;

    void setContent(const QString &content);

    const QDateTime &getDoneTime() const;

    void setDoneTime(const QDateTime &doneTime);

    const QDateTime &getStartTime() const;

    void setStartTime(const QDateTime &startTime);

    bool isStar() const;

    void setStar(bool star);

    // ------

    TodoItem() = delete;

    TodoItem(const QString &title, const QString &des, const QString &content,
             const QDateTime &startTime, const QDateTime doneTime, bool star);

private:

    QString title;
    QString description;
    QString content;
    QDateTime startTime;
    QDateTime doneTime;
    bool star;

};
