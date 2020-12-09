//
// Created by justin on 2020/11/18.
//
#pragma once

#include <QWidget>

class PDFProcessor : public QWidget {
Q_OBJECT

public:

    PDFProcessor();

    PDFProcessor(const PDFProcessor &) = delete;

    PDFProcessor &operator=(const PDFProcessor &) = delete;

private slots:

    void loadPDF();

    void renderPDF();

private:
    // actions
    QAction *loadPDFAct;
    QAction *renderPDFAct;


};
