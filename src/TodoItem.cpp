//
// Created by justin on 2020/11/13.
//
#include "../include/TodoItem.h"

TodoItem::TodoItem(const QString &title, const QString &des, const QString &content, const QDateTime &startTime,
                   const QDateTime doneTime, bool star) :
        title(title), description(des), content(content), startTime(startTime), doneTime(doneTime), star(star) {
}

const QString &TodoItem::getTitle() const {
    return title;
}

void TodoItem::setTitle(const QString &title) {
    TodoItem::title = title;
}

const QString &TodoItem::getDescription() const {
    return description;
}

void TodoItem::setDescription(const QString &description) {
    TodoItem::description = description;
}

const QString &TodoItem::getContent() const {
    return content;
}

void TodoItem::setContent(const QString &content) {
    TodoItem::content = content;
}

const QDateTime &TodoItem::getDoneTime() const {
    return doneTime;
}

void TodoItem::setDoneTime(const QDateTime &doneTime) {
    TodoItem::doneTime = doneTime;
}

const QDateTime &TodoItem::getStartTime() const {
    return startTime;
}

void setStartTime(const QDateTime &startTime) {
    TodoItem::startTime = startTime;
}

bool TodoItem::isStar() const {
    return star;
}

void TodoItem::setStar(bool star) {
    TodoItem::star = star;
}
