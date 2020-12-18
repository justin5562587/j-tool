//
// Created by justin on 2020/12/18.
//

#pragma once

#include <QComboBox>
#include <QtPdfWidgets/qpdfview.h>

class ZoomSelector : public QComboBox
{
Q_OBJECT

public:
    explicit ZoomSelector(QWidget *parent = nullptr);

public slots:
    void setZoomFactor(qreal zoomFactor);

    void reset();

signals:
    void zoomModeChanged(QPdfView::ZoomMode zoomMode);
    void zoomFactorChanged(qreal zoomFactor);

private slots:
    void onCurrentTextChanged(const QString &text);
};
