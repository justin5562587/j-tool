//
// Created by justin on 2020/12/21.
//

#include "PDFMain.h"

#include <QLabel>

PDFMain::PDFMain(QWidget* parent) : QWidget(parent) {
    QWidget* content = new QWidget;
    QLabel* title = new QLabel;
    title->setText("PDF Main");
}
