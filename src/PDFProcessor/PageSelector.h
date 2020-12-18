//
// Created by justin on 2020/12/18.
//

#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPdfDocument;
class QPdfPageNavigation;
class QToolButton;

class PageSelector : public QWidget
{
Q_OBJECT

public:
    explicit PageSelector(QWidget *parent = nullptr);

    void setPageNavigation(QPdfPageNavigation *pageNavigation);

private slots:
    void onCurrentPageChanged(int page);
    void pageNumberEdited();

private:
    QPdfPageNavigation *m_pageNavigation;

    QLineEdit *m_pageNumberEdit;
    QLabel *m_pageCountLabel;
    QToolButton *m_previousPageButton;
    QToolButton *m_nextPageButton;
};
